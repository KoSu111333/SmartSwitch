#include "drv_rcc.h"
volatile static uint32_t systick_ms = 0;
volatile static uint32_t ex_systick_ms = 0;
volatile static uint32_t timeout_s = 0;

void SysTick_Init_IRQ(uint32_t ticks) {
    SysTick->CTRL = 0;
    if (ticks > SysTick_LOAD_RELOAD_Msk) ticks = SysTick_LOAD_RELOAD_Msk;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    // NVIC 우선순위 설정은 플랫폼에 따라 필요
}

/**
  * @brief  Stop Mode 진입
  * @param  None
  * @retval None
  */
void enter_stop_mode(void) {
    // 모든 인터럽트 활성화 (Stop Mode에서 깨어나기 위해)
    __enable_irq();

    // 1. WFI (Wait For Interrupt) 모드에서 deepsleep 활성화
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // 2. PWR Control Register (PWR->CR) 설정
    //    a. PDDS (Power Down Deep Sleep) 비트 설정: Stop Mode 진입
    PWR->CR |= PWR_CR_PDDS;

    //    b. LPDS (Low Power Deep Sleep) 비트 설정: Stop 모드에서 레귤레이터 동작 모드 선택
    //       - LPDS = 1: Low-power regulator ON during Stop mode (SRAM, Regs content preserved, slower wake-up)
    //       - LPDS = 0: Main regulator ON during Stop mode (SRAM, Regs content preserved, faster wake-up, higher consumption)
    //       일반적으로 더 낮은 전력 소모를 위해 LPDS = 1을 선택
    PWR->CR |= PWR_CR_LPDS; // Low-power regulator enabled (최소 전력)

    // 3. WUF (Wake-up Flag) 클리어
    //    STOP Mode 진입 전에 클리어해야 Wake-up 이벤트가 정상적으로 동작
    PWR->CR |= PWR_CR_CWUF; // Write 1 to clear WUF

    // 4. __WFI() 호출: Wait For Interrupt. 여기서 Stop Mode로 진입
    __WFI();

    // 5. Stop Mode에서 깨어난 후:
    //    __WFI() 이후 코드가 실행된다는 것은 인터럽트에 의해 깨어났다는 의미
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; // SLEEPDEEP 비트 클리어

    // 클럭 재설정 (Stop Mode 복귀 시 시스템 클럭이 HSI로 바뀔 수 있음)
    // 여기서는 SystemClock_Config()를 다시 호출해서 PLL을 재설정해줄게.
    exit_stop_mode_post_processing();
}
/**
  * @brief  Stop Mode 복귀 후 처리
  *         주로 시스템 클럭 재설정을 담당
  * @param  None
  * @retval None
  */
void exit_stop_mode_post_processing(void) {
	RCC_init(); // 시스템 클럭 (PLL) 재설정
    // GPIO 초기화 재확인 (필요 시) 등
}

void set_timeout_ms(uint32_t timeout)
{
    timeout_s = timeout;
}
void time_update()
{
    ex_systick_ms = systick_ms;
}
bool is_timeout()
{
    bool ret = false;

    if (systick_ms - ex_systick_ms >= timeout_s)
    {
        ret = true;
    }
    return ret;
}
void SysTick_Handler(void) {
    systick_ms++;
}

void delay_ms_irq(uint32_t ms) {
    uint32_t target = systick_ms + ms;

    while (systick_ms < target) {
       __WFI(); // 가능하면 저전력 대기(옵션), 또는 다른 작업 수행
    }
}

void RCC_init(void)
{
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY));

    RCC->CFGR &= ~(
        RCC_CFGR_PLLSRC_Msk   |
        RCC_CFGR_PLLXTPRE_Msk |
        RCC_CFGR_PLLMULL_Msk  |
        RCC_CFGR_HPRE_Msk     |
        RCC_CFGR_PPRE1_Msk    |
        RCC_CFGR_PPRE2_Msk
    );

    FLASH->ACR &= ~(FLASH_ACR_LATENCY_Msk);
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;     // AHB  64MHz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;    // APB1 = HCLK/2 (32MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;    // APB2 = HCLK (64MHz)
    RCC->CFGR |= RCC_CFGR_PLLMULL16;     // (HSI/2=)4MHz * 16 = 64MHz
    
    RCC->CR |= RCC_CR_PLLON;    
    //wait Ready flg 
    while(!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_PLL;
    while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL); 

    RCC->BDCR |= RCC_BDCR_LSEON;
    while(!(RCC->BDCR & RCC_BDCR_LSERDY_Msk)); // LSE Ready 대기

    // 8. LSI 활성화 (내부 저속 RC 발진기)
    RCC->CSR |= RCC_CSR_LSION;
    while(!(RCC->CSR & RCC_CSR_LSIRDY)); // LSI Ready 대기

    // 1ms 
    // tick 기본 시간 단위? (1ms)
    __enable_irq(); 
    SysTick_Init_IRQ((SYSTEM_CORE_CLOCK / 1000) - 1);
}
// --- 사용자 정의 매크로 ---
// PCLK1 주파수 설정 (STM32F1에서 일반적인 SYSCLK=72MHz일 때 PCLK1=36MHz)
#define PCLK1_FREQ                  36000000UL

// WWDG 프리스케일러 값 (WWDG_CFR 레지스터의 WDGTB 비트):
// 0: div 1, 1: div 2, 2: div 4, 3: div 8
// 여기서는 3 (div8)을 사용할게.
#define WWDG_PRESCALER_VALUE        3               // WDGTB[1:0]에 0b11 설정

// WWDG 카운터 초기값 (WWDG_CR 레지스터의 T[6:0] 비트): 0x40(64) ~ 0x7F(127)
// WWDG는 T[6:0]이 0x40 아래로 내려가면 리셋을 발생시켜.
// 최대 값 0x7F (127)로 시작할게.
#define WWDG_COUNTER_INIT_VALUE     0x7F
#define WWDG_WINDOW_VALUE           0x50


/**
  * @brief  WWDG (Window Watchdog Timer) 초기화 함수
  *         PCLK1 클럭을 활성화하고, 분주율, 윈도우 값, 카운터 초기값을 설정 후 WWDG 활성화
  * @param  None
  * @retval None
  */
void WWDG_Init(void) {
    // 1. WWDG 클럭 활성화 (RCC APB1ENR 레지스터)
    //   APB1 주변장치 클럭 활성화 레지스터(APB1ENR)의 WWDGEN 비트 설정
    RCC->APB1ENR |= RCC_APB1ENR_WWDGEN;

    // 2. WWDG_CFR (Configuration Register) 설정
    //    a. WDGTB (프리스케일러 비트) 설정: PCLK1을 얼마나 나눌지 결정 (WWDG_CFR_WDGTB_x)
    //       WWDG_PRESCALER_VALUE를 사용하여 설정.
    WWDG->CFR &= ~WWDG_CFR_WDGTB; // 기존 WDGTB 비트 초기화
    WWDG->CFR |= (WWDG_PRESCALER_VALUE << WWDG_CFR_WDGTB_Pos);

    //    b. W (윈도우 값) 설정: W[6:0] 비트.
    //       카운터가 이 값보다 작아졌을 때 재설정하면 리셋 발생.
    WWDG->CFR &= ~WWDG_CFR_W; // 기존 W 비트 초기화
    WWDG->CFR |= WWDG_WINDOW_VALUE;

    // 3. WWDG_CR (Control Register) 설정
    //    T (카운터 초기값) 설정: T[6:0] 비트.
    //    이 값은 반드시 WWDG_WINDOW_VALUE보다 커야 하며, 0x40(64) ~ 0x7F(127) 범위여야 해.
    WWDG->CR &= ~WWDG_CR_T; // 기존 T 비트 초기화
    WWDG->CR |= WWDG_COUNTER_INIT_VALUE;

    // 4. WDGA (WWDG 활성화) 비트 설정: 마지막으로 WWDG를 활성화
    WWDG->CR |= WWDG_CR_WDGA;
}

/**
  * @brief  WWDG 카운터를 재설정 (Feed) 함수
  *         WWDG 타이머를 다시 시작하여 시스템 리셋을 방지
  *         이 함수는 반드시 WWDG 윈도우(W) 값보다 카운터 값이 클 때 호출되어야 해.
  * @param  None
  * @retval None
  */
void WWDG_Feed(void) {
    // WWDG_CR 레지스터의 T[6:0] 비트에 새로운 카운터 값 쓰기
    // WWDG_COUNTER_INIT_VALUE 값을 다시 써서 타이머를 재시작해.
    // 이때 T[6]은 반드시 1이어야 하므로 0x40(64) 이상의 값을 써야 해.
    WWDG->CR = (WWDG->CR & ~WWDG_CR_T) | WWDG_COUNTER_INIT_VALUE;
}
