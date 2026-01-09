#ifndef BUFFER_H
#define BUFFER_H

#include "stdint.h"
#include "stdbool.h"

#define MAX_CIRCLE_Q_BUF_SIZE (64)

typedef struct{
    uint8_t front; // idx
    uint8_t rear; // idx
    uint8_t q_buf[MAX_CIRCLE_Q_BUF_SIZE];
}CircleQueue;


void CQ_init(volatile CircleQueue *mQ);
bool CQ_is_full(volatile CircleQueue *mQ);
bool CQ_is_empty(volatile CircleQueue *mQ);
bool CQ_enqueue(volatile CircleQueue *mQ,uint8_t data);
uint8_t CQ_dequeue(volatile CircleQueue *mQ);
uint8_t CQ_top(volatile CircleQueue *mQ);
uint32_t CQ_get_buf_len(volatile CircleQueue* mQ);

void CQ_buf_clear(volatile CircleQueue *mQ); // uint8_t 반환값은 필요 없음

#endif // BUFFER_H

