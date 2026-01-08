#include "debug.h"


extern bool uart_tx_ready_flg;
extern bool blue_msg_ready_flg;
extern CircleQueue blue_buffer;
extern CircleQueue uart_buffer;



void uart_cmd_mode()
{
    while(true)
    {
       
       if (uart_tx_ready_flg)
       {
            uint8_t buf_size = CQ_get_buf_len(&uart_buffer);

            uart_tx_ready_flg = false;
//            uart_buffer.q_buf[buf_size+1] = '\r';
//            uart_buffer.q_buf[buf_size+2] = '\n';
            blue_send_queue_data(&uart_buffer);
            uart_send_data(uart_buffer.q_buf, CQ_get_buf_len(&uart_buffer) - 1);
            UART_SEND_STRING(" 전송 완료\r\n");
        //    process_bluetooth_rx_message();
            CQ_buf_clear(&uart_buffer);

       }
       if (blue_msg_ready_flg)
       {
    	   blue_msg_ready_flg = false;
    	   UART_SEND_STRING("수신 : ");
           uart_send_data(blue_buffer.q_buf, CQ_get_buf_len(&blue_buffer));
           UART_SEND_STRING("\r\n");
           CQ_buf_clear(&blue_buffer);

       }
    }

}