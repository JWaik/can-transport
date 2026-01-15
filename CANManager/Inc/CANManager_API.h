/**
 * @file    CANManager_API.h
 * @brief   C-API wrapper for CAN Manager module, Noted: This header will be added to main.c, so compiler will be treated .h as C lang.
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Jatuporn Waikoonvet
 */

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f7xx_hal.h"

void CANManager_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2);
void CANManager_Loop_CAN1();
void CANManager_Loop_CAN2();
#ifdef __cplusplus
}
#endif