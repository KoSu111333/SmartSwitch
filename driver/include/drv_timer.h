#ifndef TIM_H
#define TIM_H


#include "stdint.h"
#include "stdbool.h"
#include "stm32f103xb.h"

typedef struct{
    TIM_TypeDef* TIMx_Instance; 
    uint16_t Prescaler;         // 프리스케일러 값
    uint16_t Period;            // ARR 값 (Auto-Reload Register)
    uint8_t  Channel;           // PWM 채널 (0=CH1, 1=CH2, 2=CH3, 3=CH4)
    uint16_t Pulse;             
}TIM_Base_TypeDef;


void TIM_PWM_Config(TIM_Base_TypeDef* TIMx, uint8_t channel, uint16_t initial_pulse);
void TIM_Base_Init(TIM_TypeDef* TIMx, uint16_t prescaler, uint16_t period);
void TIM_PWM_Init(TIM_Base_TypeDef* TIMx);
void TIM_SetPWM_Pulse(TIM_Base_TypeDef* TIMx, uint16_t chg_pulse);
void TIM_PWM_Start(TIM_Base_TypeDef* TIMx);
void TIM_PWM_Stop(TIM_Base_TypeDef* TIMx);


#endif 

