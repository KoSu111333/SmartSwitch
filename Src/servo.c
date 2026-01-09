#include "servo.h"
#include "stm32f103xb.h"


static uint8_t servo_id = 0;
void servo_init(ServoConfig *servo)
{   
    servo->id = servo_id;
    servo->cur_degree = 0;
    servo_id++;
}



void servo_toggle(ServoConfig *servo)
{
    // 0 : right, 1 : left
    // need modify
    bool cur_state = servo->on_dir;
    uint8_t wish_ms;
    if (cur_state)
    {
        wish_ms = 0;
        TIM1->CCR1 = 1000;         // 1500us -> 중립
    }
    else
    {
        wish_ms = 1;
        TIM1->CCR1 = 2000;         // 1500us -> 중립

    }
//    change_bw_ms(wish_ms);

}
