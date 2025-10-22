//
// Created by Dell on 25-7-17.
//

#ifndef CANIO_H
#define CANIO_H

#include "can.h"
#include "adcio.h"
#include "stdarg.h"
#include "string.h"

void CAN_Init();

void CAN_SendMessage(float BatteryVoltage);

#endif //CANIO_H
