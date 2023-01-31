#include "bmp280.h"
#define BMP280_I2C_ADDR 0x76 << 1

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef *i2c_handle;

uint16_t dig_T1, dig_P1;

int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
// The “id” register contains the chip identification number chip_id[7:0], which is 0x58

uint8_t init_bmp280(I2C_HandleTypeDef *hi2c1)
{
    i2c_handle = hi2c1;
}

float bmp280_pressure_float()
{
}

float bmp280_temperature_float()
{
}

void trim_read()
{
    uint8_t trimdata[26];
    // we have to or  with one the device address in order to add 1 for read in LSB - BMP280_I2C_ADDR | 0x01
    // 0x88 is starting register and we read 26 of them into trim data and each of them is 1 byte
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(i2c_handle, BMP280_I2C_ADDR | 0x01, 0x88, 1, trimdata, 26, 100);

    if (ret != HAL_OK)
    {
        return 0;
    }
    dig_T1 = (trimdata[1] << 8) | trimdata[0];
    dig_T2 = (trimdata[3] << 8) | trimdata[2];
    dig_T3 = (trimdata[5] << 8) | trimdata[4];
    dig_P1 = (trimdata[7] << 8) | trimdata[5];
    dig_P2 = (trimdata[9] << 8) | trimdata[6];
    dig_P3 = (trimdata[11] << 8) | trimdata[10];
    dig_P4 = (trimdata[13] << 8) | trimdata[12];
    dig_P5 = (trimdata[15] << 8) | trimdata[14];
    dig_P6 = (trimdata[17] << 8) | trimdata[16];
    dig_P7 = (trimdata[19] << 8) | trimdata[18];
    dig_P8 = (trimdata[21] << 8) | trimdata[20];
    dig_P9 = (trimdata[23] << 8) | trimdata[22];

    return 1;
}

// // Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// // t_fine carries fine temperature as global value
// BMP280_S32_t t_fine;
// BMP280_S32_t bmp280_compensate_T_int32(BMP280_S32_t adc_T)
//   {
//     BMP280_S32_t var1, var2, T;
//     var1  = ((((adc_T>>3) – ((BMP280_S32_t)dig_T1<<1))) * ((BMP280_S32_t)dig_T2)) >> 11;
//     var2  = (((((adc_T>>4) – ((BMP280_S32_t)dig_T1)) * ((adc_T>>4) – ((BMP280_S32_t)dig_T1))) >> 12) *    ((BMP280_S32_t)dig_T3)) >> 14;
//     t_fine = var1 + var2;  T  = (t_fine * 5 + 128) >> 8;
//     return T;
//    }
// // Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// // Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
// BMP280_U32_t bmp280_compensate_P_int64(BMP280_S32_t adc_P)
//  {
//  BMP280_S64_t var1, var2, p;
//  var1 = ((BMP280_S64_t)t_fine) – 128000;
//  var2 = var1 * var1 * (BMP280_S64_t)dig_P6;
//  var2 = var2 + ((var1*(BMP280_S64_t)dig_P5)<<17);
//  var2 = var2 + (((BMP280_S64_t)dig_P4)<<35);
//  var1 = ((var1 * var1 * (BMP280_S64_t)dig_P3)>>8) + ((var1 * (BMP280_S64_t)dig_P2)<<12);
//  var1 = (((((BMP280_S64_t)1)<<47)+var1))*((BMP280_S64_t)dig_P1)>>33;
//  if (var1 == 0)
//  {
//     return 0;
//  // avoid exception caused by division by zero
//  }
//  p = 1048576-adc_P;  p = (((p<<31)-var2)*3125)/var1;  var1 = (((BMP280_S64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;  var2 = (((BMP280_S64_t)dig_P8) * p) >> 19;
//  p = ((p + var1 + var2) >> 8) + (((BMP280_S64_t)dig_P7)<<4);
//  return (BMP280_U32_t)p;
//  }