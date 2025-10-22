//
// Created by Dell on 25-7-17.
//

#include "adcio.h"

ADC_DataType adc_data;

float adc_filter_buffer[FILTER_SIZE];
uint8_t filter_index = 0;

void ADC_Init() {
    adc_data.measuredVoltage = 0.0f;

    adc_data.overVoltageCount  = 0;
    adc_data.underVoltageCount = 0;

    adc_data.overVoltageStatus  = 0;
    adc_data.underVoltageStatus = 0;

    for (int i = 0; i < FILTER_SIZE; i++) {
        adc_filter_buffer[i] = 0.0f;
    }

    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
}

void ADC_Update() {
    adc_data.measuredVoltage = ADC_Cali(HAL_ADC_GetValue(&hadc1));

    if (adc_data.measuredVoltage >= OVERVOLTAGE_THRESHOLD) {
        if (adc_data.overVoltageCount < OVERVOLTAGE_COUNT_THRESHOLD) {
            adc_data.overVoltageCount++;
        }

        if (adc_data.overVoltageCount >= OVERVOLTAGE_COUNT_THRESHOLD) {
            adc_data.overVoltageStatus = 1;
            adc_data.underVoltageStatus = 0;
        } else {
            adc_data.overVoltageStatus = 0;
            adc_data.underVoltageStatus = 0;
        }
    }
    else if (adc_data.measuredVoltage <= UNDERVOLTAGE_THRESHOLD) {
        if (adc_data.underVoltageCount < UNDERVOLTAGE_COUNT_THRESHOLD) {
            adc_data.underVoltageCount++;
        }

        if (adc_data.underVoltageCount >= UNDERVOLTAGE_COUNT_THRESHOLD) {
            adc_data.overVoltageStatus = 0;
            adc_data.underVoltageStatus = 1;
        } else {
            adc_data.overVoltageStatus = 0;
            adc_data.underVoltageStatus = 0;
        }
    }
    else if (adc_data.measuredVoltage >= EMPTY_BATTERY_VOLTAGE
        && adc_data.measuredVoltage < FULL_BATTERY_VOLTAGE) {
        adc_data.overVoltageCount = 0;
        adc_data.underVoltageCount = 0;

        adc_data.overVoltageStatus = 0;
        adc_data.underVoltageStatus = 0;
    }
}

float ADC_Cali(int16_t value) {
    return ADC_Filter(GAIN * value + BIAS);
}

float ADC_Filter(float value) {
    adc_filter_buffer[filter_index] = value;
    filter_index = (filter_index + 1) % FILTER_SIZE;

    float sum = 0;
    for (int i = 0; i < FILTER_SIZE; i++) {
        sum += adc_filter_buffer[i];
    }
    return sum / FILTER_SIZE;
}