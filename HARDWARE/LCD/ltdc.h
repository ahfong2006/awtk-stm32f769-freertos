//////////////////////////////////////////////////////////////////////////////////	 
//
//
//LCD��������	   
//
//
//��������:2015/11/26
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#ifndef _LCD_H
#define _LCD_H
#include "sys.h"

#define LCD_LED(n)              (n?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET))   //LCD����PA8

//LCD LTDC��Ҫ������
typedef struct  
{							 
	u32 pwidth;			//LCD���Ŀ��,�̶�����,������ʾ����ı�,���Ϊ0,˵��û���κ�RGB������
	u32 pheight;		//LCD���ĸ߶�,�̶�����,������ʾ����ı�
	u16 hsw;			  //ˮƽͬ�����
	u16 vsw;			  //��ֱͬ�����
	u16 hbp;			  //ˮƽ����
	u16 vbp;			  //��ֱ����
	u16 hfp;			  //ˮƽǰ��
	u16 vfp;			  //��ֱǰ�� 
	u8 activelayer; //��ǰ����:0/1	
	u8 dir;				  //0,����;1,����;
	u16 width;			//LCD���
	u16 height;			//LCD�߶�
	u32 pixsize;		//ÿ��������ռ�ֽ���
}_ltdc_dev;  

extern _ltdc_dev lcdltdc;		            //����LCD LTDC����
extern LTDC_HandleTypeDef LTDC_Handler;	    //LTDC���
extern DMA2D_HandleTypeDef DMA2D_Handler;   //DMA2D���

#define LCD_PIXEL_FORMAT_ARGB8888       0x00    
#define LCD_PIXEL_FORMAT_RGB888         0x01    
#define LCD_PIXEL_FORMAT_RGB565         0x02       
#define LCD_PIXEL_FORMAT_ARGB1555       0x03      
#define LCD_PIXEL_FORMAT_ARGB4444       0x04     
#define LCD_PIXEL_FORMAT_L8             0x05     
#define LCD_PIXEL_FORMAT_AL44           0x06     
#define LCD_PIXEL_FORMAT_AL88           0x07      

///////////////////////////////////////////////////////////////////////
//�û��޸����ò���:

//������ɫ���ظ�ʽ,һ����RGB565
#define LCD_PIXFORMAT				LCD_PIXEL_FORMAT_RGB565	
//����Ĭ�ϱ�������ɫ
#define LTDC_BACKLAYERCOLOR			0x00000000	
//LCD֡�������׵�ַ,���ﶨ����SDRAM����.
#define LCD_FRAME_BUF_ADDR			0xC0000000  


/*����Һ�������ֲ�Ĳ�������*/
//#define HBP  lcd_param[cur_lcd].hbp		//HSYNC�����Ч����
//#define VBP  lcd_param[cur_lcd].vbp		//VSYNC�����Ч����

//#define HSW  lcd_param[cur_lcd].hsw		//HSYNC���
//#define VSW  lcd_param[cur_lcd].vsw		//VSYNC���

//#define HFP  lcd_param[cur_lcd].hfp		//HSYNCǰ����Ч����
//#define VFP  lcd_param[cur_lcd].vfp		//VSYNCǰ����Ч����

//��ɫ������
#define LTDC_R0_GPIO_PORT        	  GPIOI
#define LTDC_R0_GPIO_CLK_ENABLE()   __GPIOI_CLK_ENABLE()
#define LTDC_R0_GPIO_PIN         	 GPIO_PIN_15
#define LTDC_R0_AF			           GPIO_AF14_LTDC

#define LTDC_R1_GPIO_PORT        	 GPIOJ
#define LTDC_R1_GPIO_CLK_ENABLE()   __GPIOJ_CLK_ENABLE()
#define LTDC_R1_GPIO_PIN         	 GPIO_PIN_0
#define LTDC_R1_AF			           GPIO_AF14_LTDC

#define LTDC_R2_GPIO_PORT        	 GPIOJ
#define LTDC_R2_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R2_GPIO_PIN         	 GPIO_PIN_1
#define LTDC_R2_AF			           GPIO_AF14_LTDC

#define LTDC_R3_GPIO_PORT        	 GPIOJ
#define LTDC_R3_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R3_GPIO_PIN           GPIO_PIN_2
#define LTDC_R3_AF			           GPIO_AF14_LTDC

#define LTDC_R4_GPIO_PORT        	 GPIOJ
#define LTDC_R4_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R4_GPIO_PIN         	 GPIO_PIN_3
#define LTDC_R4_AF			           GPIO_AF14_LTDC

#define LTDC_R5_GPIO_PORT        	 GPIOJ
#define LTDC_R5_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R5_GPIO_PIN         	 GPIO_PIN_4
#define LTDC_R5_AF			           GPIO_AF14_LTDC

#define LTDC_R6_GPIO_PORT        	 GPIOJ
#define LTDC_R6_GPIO_CLK_ENABLE()	 __GPIOJ_CLK_ENABLE()
#define LTDC_R6_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_R6_AF			           GPIO_AF14_LTDC

#define LTDC_R7_GPIO_PORT        	 GPIOG
#define LTDC_R7_GPIO_CLK_ENABLE()  __GPIOG_CLK_ENABLE()
#define LTDC_R7_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_R7_AF			           GPIO_AF14_LTDC
//��ɫ������
#define LTDC_G0_GPIO_PORT        	 GPIOE
#define LTDC_G0_GPIO_CLK_ENABLE()	 __GPIOE_CLK_ENABLE()
#define LTDC_G0_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_G0_AF			           GPIO_AF14_LTDC

#define LTDC_G1_GPIO_PORT        	 GPIOE
#define LTDC_G1_GPIO_CLK_ENABLE()	 __GPIOE_CLK_ENABLE()
#define LTDC_G1_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_G1_AF			           GPIO_AF14_LTDC

#define LTDC_G2_GPIO_PORT        	 GPIOA
#define LTDC_G2_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()
#define LTDC_G2_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_G2_AF			           GPIO_AF14_LTDC

#define LTDC_G3_GPIO_PORT        	 GPIOC
#define LTDC_G3_GPIO_CLK_ENABLE()	 __GPIOC_CLK_ENABLE()
#define LTDC_G3_GPIO_PIN         	 GPIO_PIN_9
#define LTDC_G3_AF			           GPIO_AF10_LTDC

#define LTDC_G4_GPIO_PORT        	 GPIOB
#define LTDC_G4_GPIO_CLK_ENABLE()	 __GPIOB_CLK_ENABLE()
#define LTDC_G4_GPIO_PIN         	 GPIO_PIN_10
#define LTDC_G4_AF			           GPIO_AF14_LTDC

#define LTDC_G5_GPIO_PORT        	 GPIOB
#define LTDC_G5_GPIO_CLK_ENABLE()	 __GPIOB_CLK_ENABLE()
#define LTDC_G5_GPIO_PIN         	 GPIO_PIN_11
#define LTDC_G5_AF			           GPIO_AF14_LTDC

#define LTDC_G6_GPIO_PORT        	 GPIOI
#define LTDC_G6_GPIO_CLK_ENABLE()	 __GPIOI_CLK_ENABLE()
#define LTDC_G6_GPIO_PIN         	 GPIO_PIN_11
#define LTDC_G6_AF			           GPIO_AF9_LTDC

#define LTDC_G7_GPIO_PORT        	 GPIOB
#define LTDC_G7_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define LTDC_G7_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_G7_AF			           GPIO_AF14_LTDC

//��ɫ������
#define LTDC_B0_GPIO_PORT        	 GPIOJ
#define LTDC_B0_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B0_GPIO_PIN         	 GPIO_PIN_12
#define LTDC_B0_AF			           GPIO_AF14_LTDC

#define LTDC_B1_GPIO_PORT        	 GPIOJ
#define LTDC_B1_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B1_GPIO_PIN         	 GPIO_PIN_13
#define LTDC_B1_AF			           GPIO_AF14_LTDC

#define LTDC_B2_GPIO_PORT        	 GPIOJ
#define LTDC_B2_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B2_GPIO_PIN           GPIO_PIN_14
#define LTDC_B2_AF			           GPIO_AF14_LTDC

#define LTDC_B3_GPIO_PORT        	 GPIOJ
#define LTDC_B3_GPIO_CLK_ENABLE()  __GPIOJ_CLK_ENABLE()
#define LTDC_B3_GPIO_PIN         	 GPIO_PIN_15
#define LTDC_B3_AF			           GPIO_AF14_LTDC

#define LTDC_B4_GPIO_PORT        	 GPIOK
#define LTDC_B4_GPIO_CLK_ENABLE()  __GPIOK_CLK_ENABLE()
#define LTDC_B4_GPIO_PIN         	 GPIO_PIN_3
#define LTDC_B4_AF			           GPIO_AF14_LTDC

#define LTDC_B5_GPIO_PORT        	 GPIOK
#define LTDC_B5_GPIO_CLK_ENABLE()	 __GPIOK_CLK_ENABLE()
#define LTDC_B5_GPIO_PIN         	 GPIO_PIN_4
#define LTDC_B5_AF			           GPIO_AF14_LTDC

#define LTDC_B6_GPIO_PORT        	 GPIOK
#define LTDC_B6_GPIO_CLK_ENABLE()  __GPIOK_CLK_ENABLE()
#define LTDC_B6_GPIO_PIN         	 GPIO_PIN_5
#define LTDC_B6_AF			           GPIO_AF14_LTDC

#define LTDC_B7_GPIO_PORT        	 GPIOK
#define LTDC_B7_GPIO_CLK_ENABLE()  __GPIOK_CLK_ENABLE()
#define LTDC_B7_GPIO_PIN         	 GPIO_PIN_6
#define LTDC_B7_AF			           GPIO_AF14_LTDC

//�����ź���
/*����ʱ��CLK*/
#define LTDC_CLK_GPIO_PORT              GPIOI
#define LTDC_CLK_GPIO_CLK_ENABLE()      __GPIOI_CLK_ENABLE()
#define LTDC_CLK_GPIO_PIN               GPIO_PIN_14
#define LTDC_CLK_AF			                GPIO_AF14_LTDC
/*ˮƽͬ���ź�HSYNC*/
#define LTDC_HSYNC_GPIO_PORT            GPIOI
#define LTDC_HSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_HSYNC_GPIO_PIN             GPIO_PIN_12
#define LTDC_HSYNC_AF			              GPIO_AF14_LTDC
/*��ֱͬ���ź�VSYNC*/
#define LTDC_VSYNC_GPIO_PORT            GPIOI
#define LTDC_VSYNC_GPIO_CLK_ENABLE()    __GPIOI_CLK_ENABLE()
#define LTDC_VSYNC_GPIO_PIN             GPIO_PIN_13
#define LTDC_VSYNC_AF			              GPIO_AF14_LTDC

/*����ʹ���ź�DE*/
#define LTDC_DE_GPIO_PORT               GPIOK
#define LTDC_DE_GPIO_CLK_ENABLE()       __GPIOK_CLK_ENABLE()
#define LTDC_DE_GPIO_PIN                GPIO_PIN_7
#define LTDC_DE_AF			                GPIO_AF14_LTDC

/*Һ���������źţ��ߵ�ƽʹ��*/
#define LTDC_BL_GPIO_PORT               GPIOA
#define LTDC_BL_GPIO_CLK_ENABLE()       __GPIOA_CLK_ENABLE()
#define LTDC_BL_GPIO_PIN                GPIO_PIN_8

void LTDC_Switch(u8 sw);					//LTDC����
void LTDC_Layer_Switch(u8 layerx,u8 sw);	//�㿪��
void LTDC_Select_Layer(u8 layerx);			//��ѡ��
void LTDC_Display_Dir(u8 dir);				//��ʾ�������
void LTDC_Draw_Point(u16 x,u16 y,u32 color);//���㺯��
u32 LTDC_Read_Point(u16 x,u16 y);			//���㺯��
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color);			//���ε�ɫ��亯��
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);	//���β�ɫ��亯��
void LTDC_Clear(u32 color);					//��������
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr);//LTDCʱ������
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height);//LTDC�㴰������
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u32 bkcolor);//LTDC������������
void LTDC_Init(void);						//LTDC��ʼ������
#endif 
