#ifndef GPIO_H
#define GPIO_H


#include "stdint.h"
#include "stdbool.h"
#include "stm32f103xb.h"

// GPIO_CRL POS
#define GPIO_PIN_0  0x00UL
#define GPIO_PIN_1  0x01UL
#define GPIO_PIN_2  0x02UL
#define GPIO_PIN_3  0x03UL
#define GPIO_PIN_4  0x04UL
#define GPIO_PIN_5  0x05UL
#define GPIO_PIN_6  0x06UL
#define GPIO_PIN_7  0x07UL

#define GPIO_PIN_8  0x08UL
#define GPIO_PIN_9  0x09UL
#define GPIO_PIN_10  0x0AUL
#define GPIO_PIN_11  0x0BUL
#define GPIO_PIN_12  0x0CUL
#define GPIO_PIN_13  0x0DUL
#define GPIO_PIN_14  0x0EUL
#define GPIO_PIN_15  0x0FUL
// GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD 등과
// GPIO_SPEED_10MHZ, GPIO_SPEED_2MHZ, GPIO_SPEED_50MHZ 등을 매개변수로 받도록 설계

//GPIOx_CRH, CRL
#define GPIO_MODE_OUTPUT_PP         (0x0U)
#define GPIO_MODE_OUTPUT_OD         (0x1U)
#define AFIO_MODE_OUTPUT_PP         (0x2U)
#define AFIO_MODE_OUTPUT_OD         (0x2U)
#define GPIO_MODE_INPUT_PULL_UP     (0x2U)   

#define GPIO_MODE_INPUT             (0x00U)   
#define GPIO_SPEED_50MHZ            (0x3U)
#define GPIO_SPEED_2MHZ             (0x2U)
#define GPIO_SPEED_10MHZ            (0x1U)


#define GPIO_A_CLK_ON   (RCC->APB2ENR |= RCC_APB2ENR_IOPAEN)
#define GPIO_B_CLK_ON   (RCC->APB2ENR |= RCC_APB2ENR_IOPBEN)
#define GPIO_C_CLK_ON   (RCC->APB2ENR |= RCC_APB2ENR_IOPCEN)
#define GPIO_D_CLK_ON   (RCC->APB2ENR |= RCC_APB2ENR_IOPDEN)
#define GPIO_E_CLK_ON   (RCC->APB2ENR |= RCC_APB2ENR_IOPEEN)

void deinit_gpio();
void state_chg_gpio();
void set_gpio(GPIO_TypeDef *port, uint32_t pin_num, uint32_t speed,uint32_t mode);
void gpio_clk_enb();
bool is_output_pin(GPIO_TypeDef *port, uint32_t pin_num);


#endif // GPIO_H

