#ifndef RCC_H
#define RCC_H


#include "stdint.h"
#include "stdbool.h"
#include "stm32f103xb.h"

// 8MHZ HSI
#define RCC_HSE_VALUE   8000000U
#define SYSTEM_CORE_CLOCK 64000000U


void RCC_init();
void set_timeout_ms(uint32_t timeout);
bool is_timeout();
void time_update();

void enter_stop_mode(void);
void delay_ms_irq(uint32_t ms);

void WWDG_Init();
void WWDG_Feed();
#endif 

