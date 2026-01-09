#ifndef BLUETOOTH_H
#define BLUETOOTH_H

// 블루투스 매니저 
#include <stdbool.h>
#include <stdint.h>
#include "buffer.h"
// type Ask, Change 
typedef enum{
    BLUE_DISCONNECTED = 0,
    BLUE_CONNECETED
}Blue_State;


typedef struct{
    uint32_t id;
    bool is_connected;
    bool received_flg;
    bool busy_flg;
}BlueConfig;

typedef enum {
    NAME = 0,//<새 이름>: 모듈 이름 변경 (예: NAMEMyDevice).
    NAMEQ,
    PIN,//<새 PIN>: 비밀번호 변경 (예: PIN123456).
    BAUD,//<속도>: 통신 속도 변경 (예: BAUD4는 115200bps).
    BAUDQ,
    MODE,//: 동작 모드 확인 (0: Slave, 1: Master).
    ROLE,//<0 or 1>: 동작 모드 변경.
    NOTI1,//: 연결/연결 해제 알림 설정 (LED 점멸 등).
    RADD,//: 마지막으로 연결된 장치의 주소 확인.
    RENEW,//: 공장 초기화 (Factory Reset). 
}AT_CMD_Enum;
void blue_init();
void bluetooth_cmdParser();
void blue_send_queue_data(volatile CircleQueue *queue_ptr);



bool chg_bluetooth_burdrate(uint8_t buad);
bool chg_bluetooth_name(const void *name);



#endif 

