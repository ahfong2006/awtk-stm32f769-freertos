//////////////////////////////////////////////////////////////////////////////////	 
//
//
//LTDC��������	   
//
//
//��������:2015/12/11
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
#include "ltdc.h"

LTDC_HandleTypeDef  LTDC_Handler;	    //LTDC���
DMA2D_HandleTypeDef DMA2D_Handler; 	    //DMA2D���

//���ݲ�ͬ����ɫ��ʽ,����֡��������
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
	u32 ltdc_lcd_framebuf[800][480] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
#else
	u16 ltdc_lcd_framebuf[800][480] __attribute__((at(LCD_FRAME_BUF_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
#endif

u32 *ltdc_framebuf[2];					//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
_ltdc_dev lcdltdc;						//����LCD LTDC����Ҫ����

//��LCD����
//lcd_switch:1 ��,0���ر�
void LTDC_Switch(u8 sw)
{
	if(sw == 1)
	{
		__HAL_LTDC_ENABLE(&LTDC_Handler);
	}
	else if(sw == 0)
	{
		__HAL_LTDC_DISABLE(&LTDC_Handler);
	}
}

//����ָ����
//layerx:���,0,��һ��; 1,�ڶ���
//sw:1 ��;0�ر�
void LTDC_Layer_Switch(u8 layerx,u8 sw)
{
	if(sw == 1)
	{
		__HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
	}
	else if(sw == 0)
	{
		__HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
	}
	__HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}

//ѡ���
//layerx:���;0,��һ��;1,�ڶ���;
void LTDC_Select_Layer(u8 layerx)
{
	lcdltdc.activelayer = layerx;
}

//����LCD��ʾ����
//dir:0,������1,����
void LTDC_Display_Dir(u8 dir)
{
	lcdltdc.dir = dir; 	//��ʾ����
	if(dir == 0)			//����
	{
		lcdltdc.width  = lcdltdc.pheight;
		lcdltdc.height = lcdltdc.pwidth;	
	}else if(dir == 1)	//����
	{
		lcdltdc.width  = lcdltdc.pwidth;
		lcdltdc.height = lcdltdc.pheight;
	}
}

//���㺯��
//x,y:����
//color:��ɫֵ
void LTDC_Draw_Point(u16 x,u16 y,u32 color)
{ 
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
        *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x)) = color;
	}else 			//����
	{
        *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)) = color; 
	}
#else
	if(lcdltdc.dir)	//����
	{
        *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x)) = color;
	}else 			//����
	{
        *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)) = color; 
	}
#endif
}

//���㺯��
//����ֵ:��ɫֵ
u32 LTDC_Read_Point(u16 x,u16 y)
{ 
#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888
	if(lcdltdc.dir)	//����
	{
		return *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}else 			//����
	{
		return *(u32*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
#else
	if(lcdltdc.dir)	//����
	{
		return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * y + x));
	}else 			//����
	{
		return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * (lcdltdc.pheight - x - 1) + y)); 
	}
#endif 
}

//LTDC������,DMA2D���
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{ 
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout = 0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx = sx;psy = sy;
		pex = ex;pey = ey;
	}else			//����
	{
		psx = sy;psy = lcdltdc.pheight - ex - 1;
		pex = ey;pey = lcdltdc.pheight - sx - 1;
	} 
	offline = lcdltdc.pwidth - (pex - psx + 1);
	addr = ((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
	RCC->AHB1ENR |= 1 << 23;			  //ʹ��DM2Dʱ��
	DMA2D->CR     = 3 << 16;				//�Ĵ������洢��ģʽ
	DMA2D->OPFCCR = LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->OOR    = offline;				//������ƫ�� 
	DMA2D->CR    &= ~(1<<0);				//��ֹͣDMA2D
	DMA2D->OMAR   = addr;				    //����洢����ַ
	DMA2D->NLR    = (pey - psy + 1) | ((pex - psx + 1) << 16);	//�趨�����Ĵ���
	DMA2D->OCOLR  = color;				//�趨�����ɫ�Ĵ��� 
	DMA2D->CR    |= 1 << 0;				//����DMA2D
	while((DMA2D->ISR & (1 << 1)) == 0)	//�ȴ��������
	{
		timeout++;
		if(timeout > 0x1FFFFF)break;	//��ʱ�˳�
	}  
	DMA2D->IFCR |= 1 << 1;				  //���������ɱ�־ 	
}

//��ָ�����������ָ����ɫ��,DMA2D���	
//�˺�����֧��u16,RGB565��ʽ����ɫ�������.
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ�����׵�ַ
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout = 0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx = sx;psy = sy;
		pex = ex;pey = ey;
	}else			//����
	{
		psx = sy;psy = lcdltdc.pheight - ex - 1;
		pex = ey;pey = lcdltdc.pheight - sx - 1;
	}
	offline = lcdltdc.pwidth - (pex - psx + 1);
	addr = ((u32)ltdc_framebuf[lcdltdc.activelayer] + lcdltdc.pixsize * (lcdltdc.pwidth * psy + psx));
	RCC->AHB1ENR  |= 1 << 23;			  //ʹ��DM2Dʱ��
	DMA2D->CR      = 0 << 16;				//�洢�����洢��ģʽ
	DMA2D->FGPFCCR = LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->FGOR    = 0;					    //ǰ������ƫ��Ϊ0
	DMA2D->OOR     = offline;				//������ƫ�� 
	DMA2D->CR     &= ~(1<<0);				//��ֹͣDMA2D
	DMA2D->FGMAR   = (u32)color;		//Դ��ַ
	DMA2D->OMAR    = addr;				  //����洢����ַ
	DMA2D->NLR     = (pey - psy + 1) | ((pex -psx+1)<<16);	//�趨�����Ĵ��� 
	DMA2D->CR     |= 1 << 0;				//����DMA2D
	while((DMA2D->ISR & (1 << 1)) == 0)	  //�ȴ��������
	{
		timeout++;
		if(timeout > 0x1FFFFF)break;	//��ʱ�˳�
	} 
	DMA2D->IFCR |= 1 << 1;				  //���������ɱ�־  	
}  

//LCD����
//color:��ɫֵ
void LTDC_Clear(u32 color)
{
	LTDC_Fill(0,0,lcdltdc.width - 1,lcdltdc.height - 1,color);
}

//LTDCʱ��(Fdclk)���ú���
//Fvco=Fin*pllsain; 
//Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;

//Fvco:VCOƵ��
//Fin:����ʱ��Ƶ��һ��Ϊ1Mhz(����ϵͳʱ��PLLM��Ƶ���ʱ��,��ʱ����ͼ)
//pllsain:SAIʱ�ӱ�Ƶϵ��N,ȡֵ��Χ:50~432.  
//pllsair:SAIʱ�ӵķ�Ƶϵ��R,ȡֵ��Χ:2~7
//pllsaidivr:LCDʱ�ӷ�Ƶϵ��,ȡֵ��Χ:RCC_PLLSAIDIVR_2/4/8/16,��Ӧ��Ƶ2~16 
//����:�ⲿ����Ϊ25M,pllm=25��ʱ��,Fin=1Mhz.
//����:Ҫ�õ�20M��LTDCʱ��,���������:pllsain=400,pllsair=5,pllsaidivr=RCC_PLLSAIDIVR_4
//Fdclk=1*400/5/4=400/20=20Mhz
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
	
	//LTDC�������ʱ�ӣ���Ҫ�����Լ���ʹ�õ�LCD�����ֲ������ã�
  PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;	//LTDCʱ�� 	
	PeriphClkIniture.PLLSAI.PLLSAIN       = pllsain;    
	PeriphClkIniture.PLLSAI.PLLSAIR       = pllsair;  
	PeriphClkIniture.PLLSAIDivR           = pllsaidivr;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture) == HAL_OK) //��������ʱ��
    {
        return 0;   //�ɹ�
    }
    else return 1;  //ʧ��    
}

//LTDC,���մ�������,������LCD�������ϵΪ��׼
//ע��:�˺���������LTDC_Layer_Parameter_Config֮��������.
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:��Ⱥ͸߶�
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  //���ô��ڵ�λ��
    HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);//���ô��ڴ�С    
}

//LTDC,������������.
//ע��:�˺���,������LTDC_Layer_Window_Config֮ǰ����.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u32 bkcolor)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	
	pLayerCfg.WindowX0        = 0;                               //������ʼX����
	pLayerCfg.WindowY0        = 0;                               //������ʼY����
	pLayerCfg.WindowX1        = lcdltdc.pwidth;                  //������ֹX����
	pLayerCfg.WindowY1        = lcdltdc.pheight;                 //������ֹY����
	pLayerCfg.PixelFormat     = pixformat;		                   //���ظ�ʽ
	pLayerCfg.Alpha           = 255;				                     //Alphaֵ���ã�0~255,255Ϊ��ȫ��͸��
	pLayerCfg.Alpha0          = 0;			                         //Ĭ��Alphaֵ
	pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;        //���ò���ϵ��
	pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;	   //���ò���ϵ��
	pLayerCfg.FBStartAdress   = bufaddr;	                       //���ò���ɫ֡������ʼ��ַ
	pLayerCfg.ImageWidth      = lcdltdc.pwidth;                  //������ɫ֡�������Ŀ��    
	pLayerCfg.ImageHeight     = lcdltdc.pheight;                 //������ɫ֡�������ĸ߶�
	pLayerCfg.Backcolor.Red   = (u8)(bkcolor & 0x00FF0000) >> 16;//������ɫ��ɫ����
	pLayerCfg.Backcolor.Green = (u8)(bkcolor & 0x0000FF00) >> 8; //������ɫ��ɫ����
	pLayerCfg.Backcolor.Blue  = (u8)bkcolor & 0x000000FF;        //������ɫ��ɫ����
	HAL_LTDC_ConfigLayer(&LTDC_Handler,&pLayerCfg,layerx);       //������ѡ�еĲ�
}  

//LCD��ʼ������
void LTDC_Init(void)
{   

	/* ʹ��LTDCʱ�� */
	__HAL_RCC_LTDC_CLK_ENABLE();	
	/* ʹ��DMA2Dʱ�� */
	__HAL_RCC_DMA2D_CLK_ENABLE();
	
	lcdltdc.pwidth  = 800;				//�����,��λ:����
	lcdltdc.pheight = 480;			  //���߶�,��λ:����
	lcdltdc.hsw     = 1;					//ˮƽͬ�����
	lcdltdc.vsw     = 1;					//��ֱͬ�����
	lcdltdc.hbp     = 46;					//ˮƽ����
	lcdltdc.vbp     = 23;					//��ֱ����
	lcdltdc.hfp     = 210;				//ˮƽǰ��
	lcdltdc.vfp     = 22;					//��ֱǰ��
	//��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  300/4/4,�Ż�ȽϺ�)
	LTDC_Clk_Set(396,3,RCC_PLLSAIDIVR_4); 

	#if LCD_PIXFORMAT == LCD_PIXFORMAT_ARGB8888 || LCD_PIXFORMAT == LCD_PIXFORMAT_RGB888 
	ltdc_framebuf[0] = (u32*)&ltdc_lcd_framebuf;
	lcdltdc.pixsize  = 4;				//ÿ������ռ4���ֽ�
	#else 
	lcdltdc.pixsize  = 2;				//ÿ������ռ2���ֽ�
	ltdc_framebuf[0] = (u32*)&ltdc_lcd_framebuf;
	#endif 	

	//LTDC����
	LTDC_Handler.Instance                = LTDC;
	LTDC_Handler.Init.HSPolarity         = LTDC_HSPOLARITY_AL;         //ˮƽͬ������
	LTDC_Handler.Init.VSPolarity         = LTDC_VSPOLARITY_AL;         //��ֱͬ������
	LTDC_Handler.Init.DEPolarity         = LTDC_DEPOLARITY_AL;         //����ʹ�ܼ���
	LTDC_Handler.Init.PCPolarity         = LTDC_PCPOLARITY_IPC;        //����ʱ�Ӽ���
	LTDC_Handler.Init.HorizontalSync     = lcdltdc.hsw - 1;            //ˮƽͬ�����
	LTDC_Handler.Init.VerticalSync       = lcdltdc.vsw - 1;            //��ֱͬ�����
	LTDC_Handler.Init.AccumulatedHBP     = lcdltdc.hsw + lcdltdc.hbp - 1; //ˮƽͬ�����ؿ��
	LTDC_Handler.Init.AccumulatedVBP     = lcdltdc.vsw + lcdltdc.vbp - 1; //��ֱͬ�����ظ߶�
	LTDC_Handler.Init.AccumulatedActiveW = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth - 1;//��Ч���
	LTDC_Handler.Init.AccumulatedActiveH = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight - 1;//��Ч�߶�
	LTDC_Handler.Init.TotalWidth         = lcdltdc.hsw + lcdltdc.hbp + lcdltdc.pwidth + lcdltdc.hfp - 1;   //�ܿ��
	LTDC_Handler.Init.TotalHeigh         = lcdltdc.vsw + lcdltdc.vbp + lcdltdc.pheight + lcdltdc.vfp - 1;  //�ܸ߶�
	LTDC_Handler.Init.Backcolor.Red      = 0;           //��Ļ�������ɫ����
	LTDC_Handler.Init.Backcolor.Green    = 0;           //��Ļ��������ɫ����
	LTDC_Handler.Init.Backcolor.Blue     = 0;           //��Ļ����ɫ��ɫ����
	HAL_LTDC_Init(&LTDC_Handler);

	//������
	LTDC_Layer_Parameter_Config(0,(u32)ltdc_framebuf[0],LCD_PIXFORMAT,0x000000);//���������
	LTDC_Layer_Window_Config(0,0,0,lcdltdc.pwidth,lcdltdc.pheight);	//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!

	//	LTDC_Layer_Parameter_Config(1,(u32)ltdc_framebuf[1],LCD_PIXFORMAT,127,0,6,7,0X000000);//���������
	//	LTDC_Layer_Window_Config(1,0,0,lcdltdc.pwidth,lcdltdc.pheight);	//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!

	LTDC_Display_Dir(1);			  //Ĭ������
	LTDC_Select_Layer(0); 			//ѡ���1��
	LCD_LED(1);         		    //��������
	LTDC_Clear(0xFFF00FFF);			//����
}

//LTDC�ײ�IO��ʼ����ʱ��ʹ��
//�˺����ᱻHAL_LTDC_Init()����
//hltdc:LTDC���
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  /* ʹ��LCDʹ�õ�������ʱ�� */
                          //��ɫ������
  LTDC_R0_GPIO_CLK_ENABLE();LTDC_R1_GPIO_CLK_ENABLE();LTDC_R2_GPIO_CLK_ENABLE();\
  LTDC_R3_GPIO_CLK_ENABLE();LTDC_R4_GPIO_CLK_ENABLE();LTDC_R5_GPIO_CLK_ENABLE();\
  LTDC_R6_GPIO_CLK_ENABLE();LTDC_R7_GPIO_CLK_ENABLE();LTDC_G0_GPIO_CLK_ENABLE();\
  LTDC_G1_GPIO_CLK_ENABLE();LTDC_G2_GPIO_CLK_ENABLE();LTDC_G3_GPIO_CLK_ENABLE();\
  LTDC_G4_GPIO_CLK_ENABLE();LTDC_G5_GPIO_CLK_ENABLE();LTDC_G6_GPIO_CLK_ENABLE();\
  LTDC_G7_GPIO_CLK_ENABLE();LTDC_B0_GPIO_CLK_ENABLE();LTDC_B1_GPIO_CLK_ENABLE();\
  LTDC_B2_GPIO_CLK_ENABLE();LTDC_B3_GPIO_CLK_ENABLE();LTDC_B4_GPIO_CLK_ENABLE();\
  LTDC_B5_GPIO_CLK_ENABLE();LTDC_B6_GPIO_CLK_ENABLE();LTDC_B7_GPIO_CLK_ENABLE();\
  LTDC_CLK_GPIO_CLK_ENABLE();LTDC_HSYNC_GPIO_CLK_ENABLE();LTDC_VSYNC_GPIO_CLK_ENABLE();\
  LTDC_DE_GPIO_CLK_ENABLE();/*LTDC_DISP_GPIO_CLK_ENABLE();*/LTDC_BL_GPIO_CLK_ENABLE();
/* GPIO���� */
 /* ��ɫ������ */                        
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  
  GPIO_InitStruct.Pin       = LTDC_R0_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R0_AF;
  HAL_GPIO_Init(LTDC_R0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = LTDC_R1_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R1_AF;
  HAL_GPIO_Init(LTDC_R1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin       = LTDC_R2_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R2_AF;
  HAL_GPIO_Init(LTDC_R2_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin       = LTDC_R3_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R3_AF;
  HAL_GPIO_Init(LTDC_R3_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin       = LTDC_R4_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R4_AF;
  HAL_GPIO_Init(LTDC_R4_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin       = LTDC_R5_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R5_AF;
  HAL_GPIO_Init(LTDC_R5_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_R6_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R6_AF;
  HAL_GPIO_Init(LTDC_R6_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_R7_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_R7_AF;
  HAL_GPIO_Init(LTDC_R7_GPIO_PORT, &GPIO_InitStruct);
  
  //��ɫ������
  GPIO_InitStruct.Pin 			= LTDC_G0_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_G0_AF;
  HAL_GPIO_Init(LTDC_G0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin 			= LTDC_G1_GPIO_PIN; 
  GPIO_InitStruct.Alternate = LTDC_G1_AF;
  HAL_GPIO_Init(LTDC_G1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_G2_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G2_AF;
  HAL_GPIO_Init(LTDC_G2_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_G3_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G3_AF;
  HAL_GPIO_Init(LTDC_G3_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_G4_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G4_AF;
  HAL_GPIO_Init(LTDC_G4_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_G5_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G5_AF;
  HAL_GPIO_Init(LTDC_G5_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_G6_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G6_AF;
  HAL_GPIO_Init(LTDC_G6_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_G7_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_G7_AF;
  HAL_GPIO_Init(LTDC_G7_GPIO_PORT, &GPIO_InitStruct);
  
  //��ɫ������
  GPIO_InitStruct.Pin 			= LTDC_B0_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B0_AF;
  HAL_GPIO_Init(LTDC_B0_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin 			= LTDC_B1_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B1_AF;
  HAL_GPIO_Init(LTDC_B1_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_B2_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B2_AF;
  HAL_GPIO_Init(LTDC_B2_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_B3_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B3_AF;
  HAL_GPIO_Init(LTDC_B3_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_B4_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B4_AF;
  HAL_GPIO_Init(LTDC_B4_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_B5_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B5_AF;
  HAL_GPIO_Init(LTDC_B5_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_B6_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B6_AF;
  HAL_GPIO_Init(LTDC_B6_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin				= LTDC_B7_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_B7_AF;
  HAL_GPIO_Init(LTDC_B7_GPIO_PORT, &GPIO_InitStruct);
  
  //�����ź���
  GPIO_InitStruct.Pin 			= LTDC_CLK_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_CLK_AF;
  HAL_GPIO_Init(LTDC_CLK_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_HSYNC_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_HSYNC_AF;
  HAL_GPIO_Init(LTDC_HSYNC_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_VSYNC_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_VSYNC_AF;
  HAL_GPIO_Init(LTDC_VSYNC_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin 			= LTDC_DE_GPIO_PIN;
  GPIO_InitStruct.Alternate = LTDC_DE_AF;
  HAL_GPIO_Init(LTDC_DE_GPIO_PORT, &GPIO_InitStruct);
  
  //����BL ��Һ��ʹ���ź�DISP
//  GPIO_InitStruct.Pin = LTDC_DISP_GPIO_PIN;                             

  
//  HAL_GPIO_Init(LTDC_DISP_GPIO_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin   = LTDC_BL_GPIO_PIN;   
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;  
  HAL_GPIO_Init(LTDC_BL_GPIO_PORT, &GPIO_InitStruct);
}

