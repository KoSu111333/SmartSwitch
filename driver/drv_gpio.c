#include "drv_gpio.h"


void init_gpio()
{
    
}
void gpio_clk_enb()
{
    // 여기서 각 포트에 맞는 클럭을 활성화하는 건 쉽지 않음.
    // GPIO 클럭 활성화는 이 함수 호출 전에 해주는 것이 일반적.
    // 예: RCC_APB2ENR |= RCC_APB2ENR_IOPAEN; // GPIOA 클럭 켜기
    GPIO_A_CLK_ON;
    GPIO_B_CLK_ON;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

}
void set_gpio(GPIO_TypeDef *port, uint32_t pin_num, uint32_t speed,uint32_t mode)
{
    // 여기서 각 포트에 맞는 클럭을 활성화하는 건 쉽지 않음.
    // GPIO 클럭 활성화는 이 함수 호출 전에 해주는 것이 일반적.
    // 예: RCC_APB2ENR |= RCC_APB2ENR_IOPAEN; // GPIOA 클럭 켜기   

    uint32_t pin_config_mask = 0; // 해당 핀의 4비트 설정 값
    uint32_t temp_reg = 0; // CRH 또는 CRL 레지스터 임시 저장 변수

    pin_config_mask = (speed << 0) | (mode << 2);

    if (pin_num < 8) // CRL 레지스터 (Pin 0 ~ 7)
    {
        temp_reg = port->CRL;
        // 해당 핀의 기존 4비트 설정 값 클리어
        temp_reg &= ~(0xF << (pin_num * 4));
        // 새로운 설정 값 적용
        temp_reg |= (pin_config_mask << (pin_num * 4));
        port->CRL = temp_reg;
    }
    else // CRH 레지스터 (Pin 8 ~ 15)
    {
        temp_reg = port->CRH;
        // 해당 핀의 기존 4비트 설정 값 클리어
        temp_reg &= ~(0xF << ((pin_num - 8) * 4));
        // 새로운 설정 값 적용
        temp_reg |= (pin_config_mask << ((pin_num - 8) * 4));
        port->CRH = temp_reg;
    }
}

uint8_t gpio_set_bit(GPIO_TypeDef *port, uint32_t pin_num, uint8_t val)
{
    // LED 끄기 (PA5 Reset)
    uint8_t ret = 0;
    if (is_output_pin(port,pin_num))
    {
        ret = 1;
        if (val)
            GPIOA->BSRR = (1 << (pin_num + GPIO_BSRR_BR0_Pos)); // BR5 비트를 사용하여 PA5 핀을 끈다
        else
            GPIOA->BSRR = (1 << (pin_num + GPIO_BSRR_BS0_Pos)); // BR5 비트를 사용하여 PA5 핀을 끈다    
    }

    return ret;

}
bool is_output_pin(GPIO_TypeDef *port, uint32_t pin_num)
{
    uint32_t reg_val;   // CRL 또는 CRH 레지스터 값을 저장할 변수
    uint32_t mode_bits; // 핀의 MODE 비트 (하위 2비트)를 저장할 변수

    // 1. 핀 번호에 따라 CRL 또는 CRH 레지스터를 선택하고 값을 읽어와야 해.
    //    핀 0-7은 CRL, 핀 8-15는 CRH
    if (pin_num < 8) {
        reg_val = port->CRL; // 핀 0-7은 CRL 레지스터 사용
        // 해당 핀의 4비트 설정 블록을 가장 하위 비트 위치(0번)로 쉬프트
        reg_val >>= (pin_num * 4);
    } else {
        reg_val = port->CRH; // 핀 8-15는 CRH 레지스터 사용
        // CRH 레지스터에서는 핀 번호에서 8을 빼야 해 (예: 핀 8은 CRH에서 0번 핀처럼 취급)
        reg_val >>= ((pin_num - 8) * 4);
    }

    // 2. 이제 reg_val 변수에는 해당 핀의 4비트 설정 값 (0bCNF1CNF0MODE1MODE0)이 들어있어.
    //    우리는 이 중에서 하위 2비트인 MODE 비트만 필요해.
    mode_bits = reg_val & 0x03; // 0x03 (0b0011) 마스크를 사용하여 하위 2비트만 추출

    // 3. 추출한 MODE 비트가 0이 아니면 출력 모드야.
    //    (0b00: 입력 모드, 0b01, 0b10, 0b11: 출력 모드)
    return (mode_bits != 0x00);
}

void deinit_gpio()
{

}
void state_chg_gpio()
{

}


void gpio_led_test()
{
    
}


