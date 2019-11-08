#include "sdram.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//+-------------------+--------------------+--------------------+--------------------+
//+                       SDRAM pins assignment                                      +
//+-------------------+--------------------+--------------------+--------------------+
//| PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0 <-> FMC_A10    |
//| PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1 <-> FMC_A11    |
//| PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG4 <-> FMC_A14    |
//| PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG5 <-> FMC_A15    |
//| PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    | PG8 <-> FC_SDCLK   |
//| PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    | PG15 <-> FMC_NCAS  |
//| PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FC_NRAS   |--------------------+
//+-------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6    |
//									 | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |
//									 | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |
//									 | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
//+-------------------+--------------------+--------------------+
//| PH2 <-> FMC_SDCKE0| PI4 <-> FMC_NBL2   |
//| PH3 <-> FMC_SDNE0 | PI5 <-> FMC_NBL3   |
//| PH5 <-> FMC_SDNW  |--------------------+
//+-------------------+
//+-------------------+------------------+
//+   32-bits Mode: D31-D16              +
//+-------------------+------------------+
//| PH8  <-> FMC_D16  | PI0  <-> FMC_D24 |
//| PH9  <-> FMC_D17  | PI1  <-> FMC_D25 |
//| PH10 <-> FMC_D18  | PI2  <-> FMC_D26 |
//| PH11 <-> FMC_D19  | PI3  <-> FMC_D27 |
//| PH12 <-> FMC_D20  | PI6  <-> FMC_D28 |
//| PH13 <-> FMC_D21  | PI7  <-> FMC_D29 |
//| PH14 <-> FMC_D22  | PI9  <-> FMC_D30 |
//| PH15 <-> FMC_D23  | PI10 <-> FMC_D31 |
//+-------------------+------------------+

//+-------------------+
//+  Pins remapping   +
//+-------------------+
//| PC0 <-> FMC_SDNWE |
//| PC2 <-> FMC_SDNE0 |
//| PC3 <-> FMC_SDCKE0|
//+-------------------+	 							  
////////////////////////////////////////////////////////////////////////////////// 	
SDRAM_HandleTypeDef SDRAM_Handler;   //SDRAM���

//SDRAM��ʼ��
void SDRAM_Init(void)
{
	u32 temp = 0;
	FMC_SDRAM_TimingTypeDef SDRAM_Timing;

	SDRAM_Handler.Instance                = FMC_Bank5_6;                       //SDRAM��BANK5,6  
	SDRAM_Handler.Init.SDBank             = FMC_SDRAM_BANK1;                   //SDRAM����BANK1��
	SDRAM_Handler.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;       //������
	SDRAM_Handler.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;         //������
	SDRAM_Handler.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32;        //���ݿ��Ϊ32λ
	SDRAM_Handler.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;      //һ��4��BANK
	SDRAM_Handler.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_2;           //CASΪ2
	SDRAM_Handler.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
	SDRAM_Handler.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;          //SDRAMʱ��ΪHCLK/2=216M/2=108M=9.3ns
	SDRAM_Handler.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;           //ʹ��ͻ��
	SDRAM_Handler.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;           //��ͨ����ʱ

	SDRAM_Timing.LoadToActiveDelay    = 2;                                     //����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������
	SDRAM_Timing.ExitSelfRefreshDelay = 7;                                     //�˳���ˢ���ӳ�Ϊ8��ʱ������
	SDRAM_Timing.SelfRefreshTime      = 5;                                     //��ˢ��ʱ��Ϊ6��ʱ������                                 
	SDRAM_Timing.RowCycleDelay        = 6;                                     //��ѭ���ӳ�Ϊ6��ʱ������
	SDRAM_Timing.WriteRecoveryTime    = 2;                                     //�ָ��ӳ�Ϊ2��ʱ������
	SDRAM_Timing.RPDelay              = 2;                                     //��Ԥ����ӳ�Ϊ2��ʱ������
	SDRAM_Timing.RCDDelay             = 2;                                     //�е����ӳ�Ϊ2��ʱ������
	HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);  

	//SDRAM��������ʼ������Ժ���Ҫ��������˳���ʼ��SDRAM
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0); //ʱ������ʹ��
	delay_us(500);                                  //������ʱ200us
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0);       //�����д洢��Ԥ���
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0);//������ˢ�´��� 
	//����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
	//bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
	//bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
	temp = (u32)SDRAM_MODEREG_BURST_LENGTH_1     |	  //����ͻ������:1(������1/2/4/8)
	       SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	  //����ͻ������:����(����������/����)
	       SDRAM_MODEREG_CAS_LATENCY_2           |	  //����CASֵ:3(������2/3)
	       SDRAM_MODEREG_OPERATING_MODE_STANDARD |    //���ò���ģʽ:0,��׼ģʽ
	       SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;      //����ͻ��дģʽ:1,�������
	SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp); //����SDRAM��ģʽ�Ĵ���

	//ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
	//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
	//����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=216/2=108Mhz,����Ϊ8192(2^13).
	//����,COUNT=64*1000*108/8192-20=823
	HAL_SDRAM_ProgramRefreshRate(&SDRAM_Handler,0x056A);
}

//SDRAM�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_SDRAM_Init()����
//hsdram:SDRAM���
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
	GPIO_InitTypeDef gpio_init_structure;

	__HAL_RCC_FMC_CLK_ENABLE();     //ʹ��FMCʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();   //ʹ��GPIODʱ��
	__HAL_RCC_GPIOE_CLK_ENABLE();   //ʹ��GPIOEʱ��
	__HAL_RCC_GPIOF_CLK_ENABLE();   //ʹ��GPIOFʱ��
	__HAL_RCC_GPIOG_CLK_ENABLE();   //ʹ��GPIOGʱ��
  __HAL_RCC_GPIOH_CLK_ENABLE();   //ʹ��GPIOHʱ��
  __HAL_RCC_GPIOI_CLK_ENABLE();	  //ʹ��GPIOIʱ��

  /* GPIOD configuration */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9 | GPIO_PIN_10 |\
                                  GPIO_PIN_14 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC; 
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  /* GPIOE configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8 | GPIO_PIN_9 |\
                                  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                                  GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;    
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  
  /* GPIOF configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3 | GPIO_PIN_4 |\
                                  GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                                  GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;    
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  
  /* GPIOG configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4|\
                                  GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;															
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);

  /* GPIOH configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 |\
                                  GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |\
                                  GPIO_PIN_15;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;															
  HAL_GPIO_Init(GPIOH, &gpio_init_structure); 
  
  /* GPIOI configuration */  
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |\
                                  GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_NOPULL;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;															
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);  
}

//��SDRAM��������
//bankx:0,��BANK5�����SDRAM����ָ��
//      1,��BANK6�����SDRAM����ָ��
//cmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
//refresh:��ˢ�´���
//regval:ģʽ�Ĵ����Ķ���
//����ֵ:0,����;1,ʧ��.
u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval)
{
	u32 target_bank = 0;
	FMC_SDRAM_CommandTypeDef Command;

	if(bankx == 0)
	{
		target_bank = FMC_SDRAM_CMD_TARGET_BANK1;  
	}		
	else if(bankx == 1)
	{
		target_bank = FMC_SDRAM_CMD_TARGET_BANK2;   
	}
	Command.CommandMode            = cmd;         //����
	Command.CommandTarget          = target_bank; //Ŀ��SDRAM�洢����
	Command.AutoRefreshNumber      = refresh;     //��ˢ�´���
	Command.ModeRegisterDefinition = regval;      //Ҫд��ģʽ�Ĵ�����ֵ
	if(HAL_SDRAM_SendCommand(&SDRAM_Handler,&Command,0x1000)==HAL_OK) //��SDRAM��������
	{
		return 0;  
	}
	else
	{
		return 1;    
	}
}

//��ָ����ַ(WriteAddr+Bank5_SDRAM_ADDR)��ʼ,����д��n���ֽ�.
//pBuffer:�ֽ�ָ��
//WriteAddr:Ҫд��ĵ�ַ
//n:Ҫд����ֽ���
void FMC_SDRAM_WriteBuffer(u8 *pBuffer,u32 WriteAddr,u32 n)
{
	for(;n != 0;n--)
	{
		*(vu8*)(Bank5_SDRAM_ADDR + WriteAddr) = *pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//��ָ����ַ((WriteAddr+Bank5_SDRAM_ADDR))��ʼ,��������n���ֽ�.
//pBuffer:�ֽ�ָ��
//ReadAddr:Ҫ��������ʼ��ַ
//n:Ҫд����ֽ���
void FMC_SDRAM_ReadBuffer(u8 *pBuffer,u32 ReadAddr,u32 n)
{
	for(;n != 0;n--)
	{
		*pBuffer++ = *(vu8*)(Bank5_SDRAM_ADDR + ReadAddr);
		ReadAddr++;
	}
}
