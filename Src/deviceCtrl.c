#include "main.h"

extern CircleQueue blue_buffer;
extern CircleQueue uart_buffer;

void stop_mode()
{

}

void cmd_parser(DevConfig *dconfig)
{
    uint8_t *copy_buf;
    uint8_t cmd =  CQ_dequeue(&blue_buffer);
    uint8_t parm_size = CQ_get_buf_len(&blue_buffer);
    for (int i = 0; i < parm_size; i++)
    {
        copy_buf[i] = CQ_dequeue(&blue_buffer);
    }
    switch(cmd){
        case SWITCH_TOGGLE: 
            servo_toggle(&dconfig->sConfig);
            break;
        case CHG_NAME:
            bluetooth_sendCmdParser(NAME,(void*)copy_buf,parm_size);
        // case CHG_PW:
            // bluetooth_sendCmdParser(ㅖ,(void*)copy_buf,parm_size);
        default:
            break;
    }

}
