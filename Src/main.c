#include "main.h" // 여기에 stm32f103xb.h 등이 포함된다고 가정

volatile CircleQueue blue_buffer;
volatile CircleQueue uart_buffer;
volatile TIM_Base_TypeDef Servo_Timer;
volatile DevConfig dConfig;
volatile bool uart_tx_ready_flg = false;
volatile bool blue_msg_ready_flg = false;

#define DEBUG_FLG  1

void servo_module_init()
{
    Servo_Timer.TIMx_Instance = SERVO_TIMER;
    Servo_Timer.Prescaler = 64-1;
    Servo_Timer.Prescaler = 20000-1;
    Servo_Timer.Channel = SERVO_TIMER_CHANNEL;
    set_gpio(SERVO_PORT, SERVO_PIN,GPIO_SPEED_2MHZ,AFIO_MODE_OUTPUT_PP);
    TIM_PWM_Init(&Servo_Timer);
}
void bluetooth_module_init()
{
    set_gpio(BLUETOOTH_UART_PORT,BLUETOOTH_UART_RX_PIN,0x00,0x1);
    set_gpio(BLUETOOTH_UART_PORT,BLUETOOTH_UART_TX_PIN,GPIO_SPEED_50MHZ, AFIO_MODE_OUTPUT_PP);
    uart_init(BLUETOOTH_UART);
    blue_init();
}

void IF_buffer_init()
{
    CQ_init(&blue_buffer);
    CQ_init(&uart_buffer);
}

void drv_init()
{
    RCC_init(); 
    gpio_clk_enb();
    IF_buffer_init();
    servo_module_init();
    bluetooth_module_init();
#ifdef DEBUG_FLG
    set_gpio(GPIOA, 2U, GPIO_SPEED_50MHZ, AFIO_MODE_OUTPUT_PP); 
    set_gpio(GPIOA, 3U, 0x00, 0x1); // 또는 올바른 Input Floating 매크로 사용 (예: GPIO_MODE_INPUT_FLOATING)
    uart_init(USART2);
    uart_cmd_mode();
#endif

    UART_SEND_STRING("SMART SWITCH \r\n");
    UART_SEND_STRING("VER : 00.00 \r\n");

    return;
}
uint8_t main()
{
    drv_init();
    TIM_PWM_Start(&Servo_Timer);
    while(1)
    {
        for(int i = 0 ; i < 100 ; i++)
        {
            TIM_PWM_Start(i);
            delay_ms_irq(1000);
        }
    }
    idle();

    return 0;
}


void idle()
{
    // 타임아웃 10000
    time_update();

    set_timeout_ms(100000);

    while(true)
    {
        // 10s
        if (is_timeout())
        {
            // 10초간 무응답일 경우 STOP 모드
            enter_stop_mode();
        }
        if (dConfig.connection == BLUE_DISCONNECTED)
        {
            while(dConfig.connection == BLUE_CONNECETED){};
            dConfig.state = READY;
        }
        if (dConfig.state == READY)
        {
            connected();
        }

    }

}


void connected()
{
    while(true)
    {
        if (dConfig.state = READY)
        {
            if (blue_msg_ready_flg)
            {
                cmd_parser(&blue_buffer);
            }
        }

        if(dConfig.state = BUSY)
        {
            //noting
        };

        if(dConfig.state = IDLE)
        {
            dConfig.state = IDLE;
        };

        if (dConfig.state == IDLE) return;

    }

}
