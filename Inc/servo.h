#ifndef SERVO_H
#define SERVO_H

#include "drv_timer.h"
// 서보 매니저
// 지금 각도가 어떻게 되어있는지 상황 
// 눌렸는지 안눌렸는지 

typedef struct{
    uint8_t id;
    uint8_t on_dir; // default right 
    uint16_t cur_degree;
}ServoConfig;

void servo_init(ServoConfig *servo);
void servo_toggle(ServoConfig *servo);






#endif 

