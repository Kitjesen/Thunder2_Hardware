//
// Created by Dell on 25-7-17.
//

#include "canio.h"

#include "uartio.h"

void CAN_Init() {
    CAN_FilterTypeDef can_filter_t;
    can_filter_t.FilterBank = 0;
    can_filter_t.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_t.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_t.FilterIdHigh = 0x0000;
    can_filter_t.FilterIdLow = 0x0000;
    can_filter_t.FilterMaskIdHigh = 0x0000;
    can_filter_t.FilterMaskIdLow = 0x0000;
    can_filter_t.FilterActivation = ENABLE;
    can_filter_t.FilterFIFOAssignment = CAN_RX_FIFO0;
    can_filter_t.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan, &can_filter_t);
    HAL_CAN_Start(&hcan);
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

void CAN_SendMessage(float BatteryVoltage) {
    CAN_TxHeaderTypeDef tx_header;
    uint32_t tx_mailbox;
    uint8_t tx_data[4];

    tx_header.StdId = 0x1EE;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = 0x04;

    memcpy(tx_data, &BatteryVoltage, sizeof(BatteryVoltage));

    HAL_CAN_AddTxMessage(&hcan, &tx_header, tx_data, &tx_mailbox);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);

    if (hcan->Instance == CAN1) {

    }
}
