#ifndef I2C_H
#define I2C_H


#include "stdint.h"
#include "stdbool.h"
#include "stm32f103xb.h"


void i2c_init();
void i2c_deinit();
void i2c_send();
void i2c_received();

#endif 

