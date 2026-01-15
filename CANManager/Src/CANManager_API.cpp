/**
 * @file    CANManager_API.cpp
 * @brief   C-API wrapper for CAN Manager module
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Jatuporn Waikoonvet
 */

#include "CANManager.h"

static CANManager canMgr;
extern "C"
{

void CANManager_Init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2)
{
    canMgr.init(hcan1, hcan2);
}

void CANManager_Loop_CAN1()
{
    canMgr.loop_can1();
}

void CANManager_Loop_CAN2()
{
    canMgr.loop_can2();
}
}