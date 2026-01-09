#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

#include <stdint.h>
#include "drv_rcc.h"
#include "drv_gpio.h"
#include "drv_i2c.h"
#include "drv_timer.h"
#include "drv_uart.h"
#include "buffer.h"

/* ---------- Clock (Hz) ----------
   These should match your RCC_init() and SystemCoreClock settings.
   If you rely on SystemCoreClock at runtime, consider using that instead.
*/
#define PCLK1_FREQ      32000000U  // Hz
#define PCLK2_FREQ      64000000U  // Hz
#define FCLK_FREQ       64000000U  // Hz
#define SYS_FREQ        64000000U  // Hz

/* ---------- Servo (TIM1 CH1 on PA8) ----------
   TIM: TIM1 (Advanced timer, BDTR.MOE required)
   PIN: PA8 (TIM1_CH1)
   Channel: TIM_CHANNEL_1 (if using HAL-style macros)
*/
#define SERVO_TIMER             TIM1
#define SERVO_TIMER_IRQn        TIM1_UP_IRQn   // 예시: IRQ 이름이 플랫폼에 따라 다름
#define SERVO_TIMER_CHANNEL     1U
#define SERVO_PORT              GPIOA
#define SERVO_PIN               8U
#define SERVO_PIN_MASK          (1U << SERVO_PIN)

/* ---------- Bluetooth UART (USART2 on PA2/PA3) ---------- */
#define BLUETOOTH_UART          USART3
#define BLUETOOTH_UART_PORT     GPIOB
#define BLUETOOTH_UART_TX_PIN   10U
#define BLUETOOTH_UART_RX_PIN   11U

/* ---------- Utility macros ---------- */
#define MS_TO_TICKS(ms, freq_hz) ( ((uint32_t)(ms) * (uint32_t)(freq_hz) + 999U) / 1000U )
/* example: MS_TO_TICKS(20, 1000) -> ticks for 20ms at 1kHz tick */

#endif // PROJECT_DEFINES_H