/**
 * @file    CANDriver.h
 * @brief   Base class for CAN driver
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Jatuporn Waikoonvet
 */
#pragma once
#if CANMGR_ENABLED
#include <cstdint>

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f7xx_hal.h"
#ifdef __cplusplus
}
#endif

class CANManager;

class CANDriver
{
public:

    friend class CANManager;

    // init method for protocol drivers, specify driver index
    virtual void init(uint8_t driver_index) = 0;

    // link protocol drivers with interfaces
    virtual bool add_interface(CAN_HandleTypeDef* hcan_iface) = 0;

    // handler for outgoing frames
    virtual bool write_frame(CAN_TxHeaderTypeDef *tx_header,
                             const uint8_t tx_data[],
                             uint32_t *tx_mailbox,
                             const uint32_t timeout_us) { return false; }
};
#endif // CANMGR_ENABLED
