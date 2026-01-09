#include <stdio.h>
#include "drv_timer.h"

void TIM_Base_Init(TIM_TypeDef* TIMx, uint16_t prescaler, uint16_t period) {
    // 0) RCC: TIMx 클럭 활성화
    if (TIMx == TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    else if (TIMx == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // TIM2는 APB1
    else if (TIMx == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; // TIM3는 APB1
    else if (TIMx == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; // TIM4는 APB1
    // ... 다른 타이머들도 추가

    // 2) 타이머 리셋/기본화 (옵션)
    TIMx->CR1 = 0; // Control Register 1
    TIMx->CR2 = 0; // Control Register 2
    TIMx->SMCR = 0; // Slave Mode Control Register (주로 마스터/슬레이브 모드에서 사용)
    TIMx->DIER = 0; // DMA/Interrupt Enable Register
    TIMx->CNT = 0;  // Counter Register 초기화

    // 3) PSC / ARR: 클럭 분주 및 주기 설정
    TIMx->PSC = prescaler; // 프리스케일러 설정
    TIMx->ARR = period;    // 자동 리로드 레지스터 (주기) 설정

    // 7) ARPE (Auto-Reload Preload Enable) 활성화
    // ARR 값 변경 시 다음 업데이트 이벤트에 반영되도록 함. (안정성)
    TIMx->CR1 |= TIM_CR1_ARPE;
}


void TIM_PWM_Config(TIM_Base_TypeDef* TIMx, uint8_t channel, uint16_t initial_pulse) {
    uint32_t ccmr_mask_offset = 0;
    uint32_t ccmr_mode_bits = (6U << 4) | (1U << 3); // OCxM=110 (PWM Mode 1), OCxPE=1 (Output Compare Preload Enable)
    TIM_TypeDef *instance = TIMx->TIMx_Instance;
    // 5) CCMR1/CCMR2 레지스터 설정: CHx PWM mode1, OCxPE = 1
    // 채널에 따라 CCMR1 또는 CCMR2 사용, 비트 오프셋 다름
    if (channel == 0) { // CH1
        instance->CCMR1 &= ~(TIM_CCMR1_OC1M | TIM_CCMR1_OC1PE); // Clear OC1M and OC1PE
        instance->CCMR1 |= ccmr_mode_bits;
        instance->CCR1 = initial_pulse;
    } else if (channel == 1) { // CH2
        instance->CCMR1 &= ~(TIM_CCMR1_OC2M | TIM_CCMR1_OC2PE); // Clear OC2M and OC2PE
        instance->CCMR1 |= (ccmr_mode_bits << 8); // CCMR1 레지스터에서 CH2 비트들은 CH1 비트로부터 8비트 오프셋
        instance->CCR2 = initial_pulse;
    } else if (channel == 2) { // CH3
        instance->CCMR2 &= ~(TIM_CCMR2_OC3M | TIM_CCMR2_OC3PE); // Clear OC3M and OC3PE
        instance->CCMR2 |= ccmr_mode_bits;
        instance->CCR3 = initial_pulse;
    } else if (channel == 3) { // CH4
        instance->CCMR2 &= ~(TIM_CCMR2_OC4M | TIM_CCMR2_OC4PE); // Clear OC4M and OC4PE
        instance->CCMR2 |= (ccmr_mode_bits << 8); // CCMR2 레지스터에서 CH4 비트들은 CH3 비트로부터 8비트 오프셋
        instance->CCR4 = initial_pulse;
    }

    // 6) CCER 레지스터 설정: CHx 출력 활성화, active-high
    // CC1E 비트, CC2E 비트 등 채널별 비트 활성화
    uint16_t ccer_channel_enable_bit;
    uint16_t ccer_channel_polarity_bit;

    switch (channel) {
        case 0: ccer_channel_enable_bit = TIM_CCER_CC1E; ccer_channel_polarity_bit = TIM_CCER_CC1P; break;
        case 1: ccer_channel_enable_bit = TIM_CCER_CC2E; ccer_channel_polarity_bit = TIM_CCER_CC2P; break;
        case 2: ccer_channel_enable_bit = TIM_CCER_CC3E; ccer_channel_polarity_bit = TIM_CCER_CC3P; break;
        case 3: ccer_channel_enable_bit = TIM_CCER_CC4E; ccer_channel_polarity_bit = TIM_CCER_CC4P; break;
        default: return;
    }

    instance->CCER &= ~ccer_channel_polarity_bit; // Polarity: CCxP = 0 (Active High)
    instance->CCER |= ccer_channel_enable_bit;    // Output: CCxE = 1 (Channel Enable)

    // 8) BDTR: Main Output Enable (TIM1/TIM8 같은 고급 타이머에만 필요)
    // TIM1_BDTR_MOE 비트는 고급 제어 타이머 (TIM1, TIM8)에만 존재
    if (instance == TIM1 ){
        // || instance == TIM8) {
        instance->BDTR &= ~TIM_BDTR_MOE; // Clear MOE
        instance->BDTR |= TIM_BDTR_MOE;  // Set MOE
    }
}

void TIM_PWM_Init(TIM_Base_TypeDef* TIMx) {
    if (TIMx == NULL) return;

    // 2. 타이머 기본 설정 (클럭, PSC, ARR)
    TIM_Base_Init(TIMx->TIMx_Instance, TIMx->Prescaler, TIMx->Period);

    // 3. PWM 모드 관련 설정 (CCMR, CCER, 초기 펄스, BDTR)
    TIM_PWM_Config(TIMx->TIMx_Instance, TIMx->Channel, TIMx->Prescaler/2);

    // 9) 강제 업데이트 이벤트 발생 (레지스터 설정 즉시 반영)
    TIMx->TIMx_Instance->EGR = TIM_EGR_UG;

}

void TIM_SetPWM_Pulse(TIM_Base_TypeDef* TIMx, uint16_t chg_pulse) {
    // 펄스 값은 Period 값보다 클 수 없도록 유효성 검사 (옵션)
    TIM_TypeDef *TIM = TIMx;
    uint8_t channel = TIMx->Channel;

    if (chg_pulse > TIM->ARR) {
        chg_pulse = TIM->ARR;
    }
    switch (channel) {
        case 0: TIM->CCR1 = chg_pulse; break;
        case 1: TIM->CCR2 = chg_pulse; break;
        case 2: TIM->CCR3 = chg_pulse; break;
        case 3: TIM->CCR4 = chg_pulse; break;
        default: break;
    }
}

void TIM_PWM_Start(TIM_Base_TypeDef* TIMx) {
    TIMx->TIMx_Instance->CR1 |= TIM_CR1_CEN; // Counter Enable
}

void TIM_PWM_Stop(TIM_Base_TypeDef* TIMx) {
    TIMx->TIMx_Instance->CR1 &= ~TIM_CR1_CEN; // Counter Disable
}

