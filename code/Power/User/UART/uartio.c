//
// Created by Dell on 25-7-17.
//

#include "uartio.h"

#define TX_BUF_SIZE 512
uint8_t send_buff[TX_BUF_SIZE];

void UART_Print(const char *format, ...)
{
    va_list args;
    uint32_t length;

    va_start(args, format);
    length = vsnprintf((char *)send_buff, TX_BUF_SIZE, (const char *)format, args);
    va_end(args);

    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)send_buff, length);
}