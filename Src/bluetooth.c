#include "bluetooth.h"
#include "common_define.h"
#include "drv_gpio.h"



typedef enum{
    ASK = 0,
    CHG = 1
}CmdType;

static void blue_send_data( void *msg,uint8_t size);

//extern uart_tx_ready_flg;
void blue_init()
{

}

void process_bluetooth_rx_message(CircleQueue *buf) // 인자도 현재 로직에서는 불필요해 보임.
{
    char received_msg_buffer[64]; // 큐에서 꺼낸 데이터를 저장할 버퍼 (스택에 할당)
    uint32_t received_len = 0; // 실제로 수신된 메시지의 길이

    // 큐가 비어있지 않고, 버퍼에 공간이 있는 동안 데이터를 읽어옴
    while (!CQ_is_empty(buf) && received_len < (64 - 1))
    {
        received_msg_buffer[received_len] = CQ_dequeue(buf); // 큐에서 데이터 빼내서 저장
        received_len++; // 길이 증가
    }
    received_msg_buffer[received_len] = '\0'; // 문자열 끝을 표시 (Null-termination)

    // 받은 메시지를 PC (VCP)로 에코 (디버깅 목적)
    #ifdef DEBUG_FLG
        // VCP_SEND_STR("FROM BLUE: "); // 매크로 사용 예시
        uart_send_data(USART2, "FROM BLUE: ", strlen("FROM BLUE: "));
        uart_send_data(USART2, received_msg_buffer, received_len); // Null-termination 포함하여 보낼 필요는 없으므로 received_len
        // VCP_SEND_STR("\r\n"); // 매크로 사용 예시
        uart_send_data(USART2, "\r\n", strlen("\r\n"));
    #endif

    // HM-10으로 받은 메시지를 그대로 에코 (혹은 처리 완료 메시지)
    // BLUE_SEND_STR("RCV OK: "); // 매크로 사용 예시
    uart_send_data("RCV OK: ", strlen("RCV OK: "));
    uart_send_data(received_msg_buffer, received_len);
    uart_send_data("\r\n", strlen("\r\n"));

    // 이제 received_msg_buffer 안에 HM-10으로부터 받은 메시지가 완전한 문자열 형태로 있어.
    // 여기서 이 메시지를 파싱하거나, 조건에 따라 특정 동작을 수행할 수 있어.
    // 예: if (strstr(received_msg_buffer, "TEMP: ") != NULL) { /* 온도 처리 */ }
}

void blue_send_queue_data(volatile CircleQueue *queue_ptr) // [수정 1] 파라미터 타입을 명확히
{
    // 큐가 비어있지 않은 동안 반복
	volatile CircleQueue Copy_queue = *queue_ptr;

    while (!CQ_is_empty(&Copy_queue)) // [수정 2] 큐가 비어있는지 확인
    {
        // if(copy_q.front == copy_q.rear) break;

        uint8_t data_to_send = CQ_dequeue(&Copy_queue); // [수정 3] CQ_dequeue로 데이터 하나씩 빼냄
        // USART3의 송신 데이터 레지스터가 비워질 때까지 기다림 (TXE 플래그 확인)
        while (!(USART3->SR & USART_SR_TXE))
        {
            // 데이터 레지스터가 비워질 때까지 대기
        }

        // 현재 바이트를 데이터 레지스터(DR)에 기록하여 전송 시작
        USART3->DR = data_to_send; // [수정 4] DR에 실제 데이터 전송
    }
    // 모든 바이트가 물리적인 라인으로 전송될 때까지 대기 (TC 플래그 확인)
    while (!(USART3->SR & USART_SR_TC))
    {
        // 전송 완료 (Transmission Complete) 플래그가 1이 될 때까지 대기
    }
}
static void blue_send_data( void *msg,uint8_t size)
{
    // 큐가 비어있지 않은 동안 반복
    //                      margin
    
    uint8_t* tmp_msg_ptr = (uint8_t*)msg;   
    
    for (uint8_t i = 0 ; i < size; i++)
    {
        uint8_t data_to_send = *(tmp_msg_ptr+i);

        while (!(USART3->SR & USART_SR_TXE))
            {
                // 데이터 레지스터가 비워질 때까지 대기
            }
        // 현재 바이트를 데이터 레지스터(DR)에 기록하여 전송 시작
        USART3->DR = data_to_send; // [수정 4] DR에 실제 데이터 전송
    }
    // 모든 바이트가 물리적인 라인으로 전송될 때까지 대기 (TC 플래그 확인)
    while (!(USART3->SR & USART_SR_TC))
    {
        // 전송 완료 (Transmission Complete) 플래그가 1이 될 때까지 대기
    }
}
void chk_blue_connection()
{

}
void bluetooth_sendCmdParser(AT_CMD_Enum cmd,void* parm1,uint8_t parm_size)
{
    uint8_t tx_buffer[20];
    uint8_t *cmd_str;
    // A,T (buf_offset is two)
    uint8_t at_offset = 2;
    tx_buffer[0] = 'A'; 
    tx_buffer[1] = 'T';

    switch (cmd)
    {
        case NAME:
        /* code */
            cmd_str = "+NAME";
            break;
        case NAMEQ:
        /* code */
            cmd_str = "+NAME?";
            break;

        case PIN:
            cmd_str = "+PIN";

        /* code */
            break;
        case BAUD:
            cmd_str = "+BAUD";

        case BAUDQ:
        /* code */
            cmd_str = "+BAUD?";
            break;

        /* code */
            break;
        case MODE:
            cmd_str = "+MODE";

        /* code */
            break;
        case ROLE:
            cmd_str = "+ROLE";

        /* code */
            break;
        case NOTI1:
            cmd_str = "+NOTI1";

        /* code */
            break;
        case RADD:
            cmd_str = "RADD";

        /* code */
            break;
        case RENEW:
            cmd_str = "RENEW";

        /* code */
            break;
        default:
            break;
    }
    uint8_t buff_size;
    if (parm_size)
    {
        for (int i = at_offset; i < parm_size + at_offset; i++)
        {
            tx_buffer[i] = cmd_str[i - at_offset];
            buff_size = i;
        }

    }

    blue_send_data(tx_buffer,buff_size);
    

}
