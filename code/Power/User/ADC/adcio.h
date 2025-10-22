//
// Created by Dell on 25-7-17.
//

#ifndef ADCIO_H
#define ADCIO_H

#include "adc.h"
#include "stm32f1xx_hal_adc_ex.h"
#include <stdint.h>

#define GAIN (0.020113528f)
#define BIAS (0.036821574f)

#define OVERVOLTAGE_THRESHOLD  (54.0f)
#define UNDERVOLTAGE_THRESHOLD (39.5f)

#define FULL_BATTERY_VOLTAGE  (53.5f)
#define EMPTY_BATTERY_VOLTAGE (41.5f)

#define OVERVOLTAGE_COUNT_THRESHOLD  (20)
#define UNDERVOLTAGE_COUNT_THRESHOLD (20)

#define FILTER_SIZE 20

typedef struct {
    float measuredVoltage;       // 当前实时电压

    uint8_t overVoltageCount;    // 过压计数器
    uint8_t underVoltageCount;   // 欠压计数器

    uint8_t overVoltageStatus;   // 过压保护状态（1为触发，0为正常）
    uint8_t underVoltageStatus;  // 欠压保护状态（1为触发，0为正常）
} ADC_DataType;

extern ADC_DataType adc_data;

void ADC_Init();

void ADC_Update();

float ADC_Cali(int16_t value);

float ADC_Filter(float value);

#endif //ADCIO_H
