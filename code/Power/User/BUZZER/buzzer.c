//
// Created by Dell on 25-7-17.
//

#include "buzzer.h"

#include "adcio.h"

uint8_t overVoltageCount = 0;
uint8_t underVoltageCount = 0;

void Buzzer_Init() {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

void Buzzer_Start() {
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim3);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, arr / 2);
    __HAL_TIM_ENABLE(&htim3);
}

void Buzzer_Stop() {
    __HAL_TIM_DISABLE(&htim3);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

void Buzzer_Play() {
    if (adc_data.overVoltageStatus) {
        underVoltageCount = 0;
        if (overVoltageCount < 80) {
            Buzzer_Start();
            overVoltageCount++;
        }
        else if (overVoltageCount < 100) {
            Buzzer_Stop();
            overVoltageCount++;
        }
        else {
            overVoltageCount = 0;
        }
    }
    else if (adc_data.underVoltageStatus) {
        overVoltageCount = 0;
        if (underVoltageCount < 20) {
            Buzzer_Start();
            underVoltageCount++;
        }
        else if (underVoltageCount < 100) {
            Buzzer_Stop();
            underVoltageCount++;
        }
        else {
            underVoltageCount = 0;
        }
    }
    else {
        overVoltageCount = 0;
        underVoltageCount = 0;
        Buzzer_Stop();
    }
}