//////////////////////////////////////////////////////////////////////////////////	 
//
//
//��ʱ����������	   
//
//
//��������:2015/11/27
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C)  2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ��3��� 
extern volatile unsigned long long FreeRTOSRunTimeTicks;

void TIM3_Init(u16 arr,u16 psc); 
void ConfigureTimeForRunTimeStats(void);
#endif

