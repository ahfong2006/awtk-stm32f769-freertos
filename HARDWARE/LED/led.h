#ifndef _LED_H
#define _LED_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//
//LED��������	   
//
//������̳:w
//��������:2015/6/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//LED�˿ڶ���
#define LED(n)		  (n?HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET))
#define LED_Toggle  (HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_12)) //LED0�����ƽ��ת


void LED_Init(void); //LED��ʼ������
#endif
