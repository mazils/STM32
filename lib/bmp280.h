#pragma once

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"
#include <math.h>

uint8_t init_bmp280(I2C_HandleTypeDef *hi2c1;);
float bmp280_pressure_float();
float bmp280_temperature_float();