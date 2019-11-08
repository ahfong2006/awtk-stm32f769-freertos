#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//
//LED��������	   
//
//
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��PC12Ϊ���.��ʹ������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();			        //����GPIOCʱ��

	GPIO_Initure.Pin   = GPIO_PIN_12;         //PC12
	GPIO_Initure.Mode  = GPIO_MODE_OUTPUT_PP; //�������
	GPIO_Initure.Pull  = GPIO_PULLUP;         //����
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;     //����
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);       //��ʼ��GPIOC.12

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);	//PB1��0
}

