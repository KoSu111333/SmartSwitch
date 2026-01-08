#include "drv_uart.h"
extern CircleQueue blue_buffer;
extern CircleQueue uart_buffer;

#define U_WORD_SIZE 8

#define PCLK1_FREQ 32000000UL
// 목표 보레이트
#define BAUD_RATE 115200UL

extern bool uart_tx_ready_flg;
extern bool blue_msg_ready_flg;
extern DevConfig dConfig;
uint16_t calculrate_buad_rate(uint32_t baud_rate);
// void uart_init()
// {
//     RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

//     // PA2 AFIO 초기화
//     GPIOA->CRL &= ~(0xF << (2 * 4)); 
//     GPIOA->CRL |= (0b1011 << (2 * 4));
//     // PA3 AFIO 초기화
//     GPIOA->CRL &= ~(0xF << (3 * 4)); 
//     GPIOA->CRL |= (0b0100 << (3 * 4)); 

//     // 보드레이트 115200 세팅 
//     USART2->BRR = calculrate_buad_rate(115200); 
//     // TX enable, RX enable
//     USART2->CR1 = (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE); 
//     // RX 인터럽트 Enable
//     USART2->CR1 |= USART_CR1_RXNEIE; 

//     // CR2 (Control Register 2) 설정: STOP 비트
//     // STOP = 00: 1 Stop bit
//     USART2->CR2 = 0x00000000; 
//     // CR3 (Control Register 3) 설정: 흐름 제어, DMA 등
//     // CTSE (CTS Enable) = 0: 흐름 제어 비활성화
//     // RTSE (RTS Enable) = 0: 흐름 제어 비활성화
//     USART2->CR3 = 0x00000000;

//     // NVIC USART2_IRQ ON
//     NVIC_EnableIRQ(USART2_IRQn);

// }
void uart_init(USART_TypeDef *USARTx)
{
    
    if (USARTx == USART2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        USARTx->BRR = calculrate_buad_rate(9600);

    }
    else if (USARTx == USART3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        USARTx->BRR = calculrate_buad_rate(9600);
    }
    // TX enable, RX enable
    USARTx->CR1 = (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE); 
    // RX 인터럽트 Enable
    USARTx->CR1 |= USART_CR1_RXNEIE; 

    // CR2 (Control Register 2) 설정: STOP 비트
    // STOP = 00: 1 Stop bit
    USARTx->CR2 = 0x00000000; 
    // CR3 (Control Register 3) 설정: 흐름 제어, DMA 등
    // CTSE (CTS Enable) = 0: 흐름 제어 비활성화
    // RTSE (RTS Enable) = 0: 흐름 제어 비활성화
    USARTx->CR3 = 0x00000000;

    // NVIC USART2_IRQ ON
    if (USARTx == USART2)
    {
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (USARTx == USART3)
    {
        NVIC_EnableIRQ(USART3_IRQn);
    }

    


}

uint16_t calculrate_buad_rate(uint32_t baud_rate) 
{
    float usartdiv = (float)PCLK1_FREQ / (16.0f * baud_rate);

    uint16_t mantissa = (uint16_t)usartdiv; 

    // fraction_float = (usartdiv - mantissa) * 16.0f;
    // 반올림은 일반적으로 (int)(값 + 0.5f) 또는 roundf() 함수를 사용
    uint16_t fraction = (uint16_t)((usartdiv - mantissa) * 16.0f + 0.5f);

    // 최종 BRR 레지스터 값 조립
    // Mantissa는 상위 12비트 (비트 15:4), Fraction은 하위 4비트 (비트 3:0)
    return (uint16_t)((mantissa << 4) | (fraction & 0xF)); // fraction은 4비트만 사용하므로 0xF 마스킹
}

/**
 * @brief 주어진 데이터를 UART로 전송합니다.
 *
 * @param data_ptr 전송할 데이터의 시작 주소 (void* 타입으로 어떤 데이터든 받을 수 있음)
 * @param data_len 전송할 데이터의 바이트 길이
 */
void uart_send_data(const void *data_ptr, uint32_t data_len)
{
    // void* 포인터를 uint8_t* (바이트 포인터)로 캐스팅하여 바이트 단위로 접근
    const uint8_t *byte_ptr = (const uint8_t *)data_ptr;

    // 데이터 길이만큼 반복하면서 한 바이트씩 전송
    for (uint32_t i = 0; i < data_len; i++)
    {
        // 1. USART2의 송신 데이터 레지스터가 비워질 때까지 기다림 (TXE 플래그 확인)
        while (!(USART2->SR & USART_SR_TXE))
        {
            // 데이터 레지스터가 비워질 때까지 대기
        }

        // 2. 현재 바이트를 데이터 레지스터(DR)에 기록하여 전송 시작
        USART2->DR = *(byte_ptr + i);
    }

    // 3. 모든 바이트가 물리적인 라인으로 전송될 때까지 대기 (TC 플래그 확인)
    while (!(USART2->SR & USART_SR_TC))
    {
        // 전송 완료 (Transmission Complete) 플래그가 1이 될 때까지 대기
    }
}
void uart3_rx_callback_fucn()
{

    if((USART3->SR & USART_SR_RXNE) != 0 )
    {
        // 1. USART2->DR 레지스터에서 데이터 읽기
        if (dConfig.state == BUSY) return;
        
        uint8_t received_data = (uint8_t)(USART3->DR & 0xFF); // 하위 8비트만 가져옴
//        blue_msg_ready_flg = true;
        if (received_data == '\n' || received_data == '\r') // 수신된 바이트가 개행 문자(종단 문자)라면
        {
            blue_msg_ready_flg = true;
        }
        if(CQ_enqueue(&blue_buffer,received_data))
		{
//            CQ_buf_clear(&blue_buffer);
        }
        else
        {
            CQ_buf_clear(&blue_buffer);
            return;
        }

    }


}
// Uart INT CALLBACK 함수 
void uart_rx_callback_fucn()
{
    // Uart INT CALLBACK 함수 
    // SR에서 새로 수신할 DR이 있을 경우
    if ((USART2->SR & USART_SR_RXNE) != 0 )
    {
        // 1. USART2->DR 레지스터에서 데이터 읽기
        uint8_t received_data = (uint8_t)(USART2->DR & 0xFF); // 하위 8비트만 가져옴

        if (received_data == '\n' || received_data == '\r') // 수신된 바이트가 개행 문자(종단 문자)라면
        {
            if (!CQ_enqueue(&uart_buffer, '\r')) {
            }
            if (!CQ_enqueue(&uart_buffer, '\n')) {
            }
            uart_tx_ready_flg = true;
        	return;
        }
        if (CQ_enqueue(&uart_buffer,received_data))
        {
        }
        else
        {
            CQ_buf_clear(&uart_buffer);
            return;

        }
    }
    
}

