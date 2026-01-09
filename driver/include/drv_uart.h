#ifndef UART_H
#define UART_H


#include "stdint.h"
#include "stdbool.h"
#include "buffer.h"
#include "main.h"
#include "stm32f103xb.h"

#define UART_SEND_STRING(STR) uart_send_data(STR,strlen(STR))
void uart_init(USART_TypeDef *USARTx);
void uart_connect();
void uart_disconnect();
void uart_send_data(const void *data_ptr, uint32_t data_len);

// received msg
void uart_rx_callback_fucn();
void uart3_rx_callback_fucn();

// typedef struct
// {
//   __IO uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
//   __IO uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
//   __IO uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
//   __IO uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
//   __IO uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
//   __IO uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
//   __IO uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
// } USART_TypeDef;

#endif 

