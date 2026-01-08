#include "buffer.h" // CircleQueue 구조체 정의 포함

// === CQ_init ===
void CQ_init(volatile CircleQueue *mQ)
{
    mQ->front = 0; // 다음에 dequeue 될 데이터가 저장될 칸의 바로 이전 칸
    mQ->rear = 0;  // 가장 최근에 enqueue 된 데이터의 위치 (데이터가 없음: rear == front)
    // 큐 초기화 시 버퍼 내용 자체를 0으로 채우는 것은 일반적으로 불필요하며, 성능에 영향을 줄 수 있음.
    // 하지만 Debugging 목적으로 초기화하는 것은 괜찮음.
    // for (int i = 0; i < MAX_CIRCLE_Q_BUF_SIZE; i++)
    // {
    //     mQ->q_buf[i] = 0;
    // }
}

// === CQ_is_empty ===
bool CQ_is_empty(volatile CircleQueue *mQ)
{
    return mQ->front == mQ->rear;
}

// === CQ_is_full ===
// 큐의 공간이 가득 찼는지 확인
// 'front'는 다음에 dequeue 될 칸의 직전, 'rear'는 마지막 enqueue 칸.
// 'rear' 다음 칸이 'front'와 같으면 꽉 참 (한 칸을 비워두는 일반적인 구현)
bool CQ_is_full(volatile CircleQueue *mQ)
{
    // (rear+1) % SIZE == front  <-- 이 로직을 사용해야 함.
    // 네 기존 코드는 오타이며, 제거되어야 함.
    // return mQ->front == (mQ->front+1) % (MAX_CIRCLE_Q_BUF_SIZE); // 이거 오타!
    return (mQ->rear + 1) % MAX_CIRCLE_Q_BUF_SIZE == mQ->front;
}

// === CQ_enqueue ===
// return : 버퍼가 꽉찼을 경우 false
bool CQ_enqueue(volatile CircleQueue *mQ,uint8_t data)
{
    if (CQ_is_full(mQ)) // 큐가 꽉 찼으면 넣을 수 없음
    {
        return false;
    }
    mQ->rear = (mQ->rear + 1) % MAX_CIRCLE_Q_BUF_SIZE; // rear를 한 칸 전진 (다음에 쓸 빈 칸으로)
    mQ->q_buf[mQ->rear] = data; // 전진한 rear 위치에 데이터 저장

    return true;
}

// === CQ_dequeue ===
// return : 버퍼가 비어있을 경우 0 (또는 다른 오류값, -1은 uint8_t에 맞지 않음)
uint8_t CQ_dequeue(volatile CircleQueue *mQ)
{
    if (CQ_is_empty(mQ)) // 큐가 비어있으면 데이터 없음
    {
        return 0; // -1은 uint8_t에 맞지 않으므로, 0 또는 별도 에러 플래그 사용
    }
    mQ->front = (mQ->front + 1) % MAX_CIRCLE_Q_BUF_SIZE; // front를 한 칸 전진 (다음에 읽을 칸으로)
    return mQ->q_buf[mQ->front]; // 전진한 front 위치의 데이터 반환
}

// === CQ_top ===
uint8_t CQ_top(volatile CircleQueue *mQ)
{
    if (CQ_is_empty(mQ))
    {
        return 0; // -1은 uint8_t에 맞지 않음
    }
    // 실제로 다음에 dequeue 될 데이터는 front 다음 칸에 있으므로!
    return mQ->q_buf[(mQ->front + 1) % MAX_CIRCLE_Q_BUF_SIZE];
}

// === CQ_buf_clear ===
// 큐를 논리적으로 비움
void CQ_buf_clear(volatile CircleQueue *mQ) // uint8_t 반환값은 필요 없음
{
    mQ->front = 0;
    mQ->rear = 0;
    // 물리적인 버퍼 클리어(q_buf를 0으로 채우는 것)는 일반적으로 필요 없음
}

// === CQ_get_buf_len (수정된 로직!) ===
uint32_t CQ_get_buf_len(volatile CircleQueue* mQ)
{
    if (mQ->rear >= mQ->front) {
        // 큐가 랩어라운드 되지 않은 경우
        // (front 바로 다음 칸부터 rear까지의 데이터 개수)
        return mQ->rear - mQ->front;
    } else {
        // 큐가 랩어라운드 된 경우
        // (버퍼 끝까지 + 버퍼 시작부터 rear까지의 데이터 개수)
        return MAX_CIRCLE_Q_BUF_SIZE - mQ->front + mQ->rear;
    }
}
