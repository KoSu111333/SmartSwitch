#ifndef DV_CTRL_H
#define DV_CTRL_H

#include "main.h"
#include "bluetooth.h"
#include "servo.h"

typedef enum{
    IDLE = 0,
    READY,
    BUSY,
    SLEEP,
    ERROR,
}DevState;

typedef enum{
    SWITCH_TOGGLE = 0,
    CHG_NAME,
    CHG_PW,
}DevCMD;

typedef struct{
    Blue_State connection;
    DevState state;
    BlueConfig bConfig;
    ServoConfig sConfig;
}DevConfig;

void cmd_parser(DevConfig *dconfig);



#endif
