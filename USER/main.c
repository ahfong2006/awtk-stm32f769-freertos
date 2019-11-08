/************************************************

************************************************/
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "bsp_touch_gtxx.h"
#include "sdram.h"
#include "ltdc.h"
#include "mpu.h"
#include "qspi.h"


#include "lcd/lcd_mem_bgr565.h"
#include "lcd/lcd_mem_bgra8888.h"
#include "main_loop/main_loop_simple.h"

#include "ff_gen_drv.h"
#include "sd_diskio_dma_rtos.h"

//�������ȼ�
#define START_TASK_PRIO			1
//�����ջ��С	
#define START_STK_SIZE 			256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LOW_TASK_PRIO			2
//�����ջ��С	
#define LOW_STK_SIZE 			256  
//������
TaskHandle_t LowTask_Handler;
//������
void low_task(void *pvParameters);

//�������ȼ�
#define MIDDLE_TASK_PRIO 		3
//�����ջ��С	
#define MIDDLE_STK_SIZE  		256 
//������
TaskHandle_t MiddleTask_Handler;
//������
void middle_task(void *pvParameters);

//�������ȼ�
#define HIGH_TASK_PRIO 			4
//�����ջ��С	
#define HIGH_STK_SIZE  			10 * 1024 
//������
TaskHandle_t HighTask_Handler;
//������
void high_task(void *pvParameters);

//��ֵ�ź������
SemaphoreHandle_t BinarySemaphore;	//��ֵ�ź���

FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL   MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */
uint8_t workBuffer[2*_MAX_SS];


extern int gui_app_start(int lcd_w, int lcd_h);

#define FB_ADDR (uint8_t*)0xC0000000

static uint8_t* s_framebuffers[2];

lcd_t* stm32f769_create_lcd(wh_t w, wh_t h) {
  lcd_t* lcd = NULL;
  uint32_t size = w * h * lcdltdc.pixsize;
  s_framebuffers[0] = FB_ADDR;
  s_framebuffers[1] = FB_ADDR + size;

#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 
  lcd = lcd_mem_bgra8888_create_double_fb(w, h, s_framebuffers[0], s_framebuffers[1]);
#else
  lcd = lcd_mem_bgr565_create_double_fb(w, h, s_framebuffers[0], s_framebuffers[1]);
#endif /*LCD_PIXFORMAT*/
	
  return lcd;
}


void SDMMC_Init_Mount(void)
{
  /*##-1- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {  
    /*##-2- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
    {
      /* FatFs Initialization Error */
      while(1);
    }
	}		
}


int main(void)
{
	MPU_Memory_Protection();        //������ش洢����
	Cache_Enable();                 //��L1-Cache
	HAL_Init();				              //��ʼ��HAL��
	Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
	delay_init(216);                //��ʱ��ʼ��
	BSP_QSPI_Init();                //��ʼ��QSPI FLASH
	BSP_QSPI_EnableMemoryMappedMode();	//ӳ��QSPI�ڴ��ַ	
	LED_Init();                     //��ʼ��LED
  SDRAM_Init();                   //SDRAM��ʼ��	
  GTP_Init_Panel();				        //��������ʼ��	
  LTDC_Init();                    //��ʼ��LCD		
	SDMMC_Init_Mount();             //SDMMC��ʼ��

	//������ʼ����
	xTaskCreate((TaskFunction_t )start_task,            //������
							(const char*    )"start_task",          //��������
							(uint16_t       )START_STK_SIZE,        //�����ջ��С
							(void*          )NULL,                  //���ݸ��������Ĳ���
							(UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
							(TaskHandle_t*  )&StartTask_Handler);   //������              
	vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();           //�����ٽ���
	
	//������ֵ�ź���
	BinarySemaphore=xSemaphoreCreateBinary();
	//��ֵ�ź��������ɹ��Ժ�Ҫ���ͷ�һ��
	if(BinarySemaphore!=NULL)xSemaphoreGive(BinarySemaphore);	
	
	//���������ȼ�����
	xTaskCreate((TaskFunction_t )high_task,             
							(const char*    )"high_task",           
							(uint16_t       )HIGH_STK_SIZE,        
							(void*          )NULL,                  
							(UBaseType_t    )HIGH_TASK_PRIO,        
							(TaskHandle_t*  )&HighTask_Handler);   
	//�����е����ȼ�����
	xTaskCreate((TaskFunction_t )middle_task,     
							(const char*    )"middle_task",   
							(uint16_t       )MIDDLE_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )MIDDLE_TASK_PRIO,
							(TaskHandle_t*  )&MiddleTask_Handler); 
//���������ȼ�����
	xTaskCreate((TaskFunction_t )low_task,     
							(const char*    )"low_task",   
							(uint16_t       )LOW_STK_SIZE,
							(void*          )NULL,
							(UBaseType_t    )LOW_TASK_PRIO,
							(TaskHandle_t*  )&LowTask_Handler);
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
}

//�����ȼ������������
void high_task(void *pvParameters)
{
	gui_app_start(lcdltdc.pwidth, lcdltdc.pheight);
	while(1)
	{
		vTaskDelay(10);	//��ʱ500ms��Ҳ����500��ʱ�ӽ���  
	}
}

//�е����ȼ������������
void middle_task(void *pvParameters)
{
	while(1)
	{    
		GT911_OnePiontScan();
		
		if (g_GT911_Point.Pressed) 
		{
			main_loop_post_pointer_event(main_loop(), TRUE, g_GT911_Point.x, g_GT911_Point.y);
		} 
		else
		{
			main_loop_post_pointer_event(main_loop(), FALSE, g_GT911_Point.x, g_GT911_Point.y);
		}		
		vTaskDelay(20);	//��ʱ1s��Ҳ����1000��ʱ�ӽ���	
	}
}

//�����ȼ������������
void low_task(void *pvParameters)
{
	while(1)
	{
		LED(0);
		vTaskDelay(300);	//��ʱ1s��Ҳ����1000��ʱ�ӽ���	
		LED(1);
		vTaskDelay(1000);	//��ʱ1s��Ҳ����1000��ʱ�ӽ���
	}
}
