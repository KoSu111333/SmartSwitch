
#include "interrupt.h"
#include "drv_gpio.h"

static volatile bool led_on = false;
static volatile servo_count = 0;
static bool servo_state = true; // false down, true u[p

void USART2_IRQHandler()
{
    uart_rx_callback_fucn();
}
void USART3_IRQHandler()
{
    uart3_rx_callback_fucn();
}

void HardFault_Handler()
{
    while(1)
    {

    }
}
void TIM1_UP_IRQHandler(void)
{
    // 1. 가장 먼저, 인터럽트 플래그를 클리어해야 해!
    // 그렇지 않으면 인터럽트가 계속 발생해!
    if (TIM1->SR & TIM_SR_UIF) // Update Interrupt Flag 확인
    {
        TIM1->CCR1 = servo_count;
        if (servo_count == 100)
        {
        	servo_state = false;
        }
        else if (servo_count == 0)
        {
        	servo_state = true;

        }
        if (servo_state)
        {
        	servo_count++;
        }
        else{
        	servo_count--;
        }
//        TIM1->SR &= ~TIM_SR_UIF; // Update Interrupt Flag 클리어

    }
}

void TIM2_IRQHandler()
{
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        if (led_on)
        {
            gpio_set_bit(GPIOA,GPIO_PIN_5,led_on);
            led_on = false;
        }
        else
        {
            gpio_set_bit(GPIOA,GPIO_PIN_5,led_on);
            led_on = true;
        }

    }

}
