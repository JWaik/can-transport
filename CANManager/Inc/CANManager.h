/**
 * @file    CANManager.h
 * @brief   CAN Manager module interface
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Jatuporn Waikoonvet
 */
#pragma once

#include "CANDriver.h"

#define MAX_CAN_DRIVERS 2
#define MAX_CAN_IFACES 2
class CANManager
{
public:
    CANManager();

    /* Do not allow copies */
    CANManager(const CANManager &other) = delete;
    CANManager &operator=(const CANManager&) = delete;

    static CANManager* get_singleton()
    {
        if (_singleton == nullptr) {
            // todo: assert some error-handler
        }
        return _singleton;
    }

    void init(void);

    //  used to identify different protocol drivers
    enum class Protocol : uint8_t {
        None = 0,
        DroneCAN = 1,
        MultiCAN = 2,
    };

    // return driver type index i
    Protocol get_driver_type(uint8_t i) const {
        if (i < sizeof(_driver_type_cache)/sizeof(_driver_type_cache[0])) {
            return _driver_type_cache[i];
        }
        return Protocol::None;
    }

private:

    CANDriver* _drivers[MAX_CAN_DRIVERS];         // Store Backend driver
    Protocol _driver_type_cache[MAX_CAN_DRIVERS]; // Cache driver protocol, use with get_driver_type()
    CAN_HandleTypeDef* _hcan[MAX_CAN_DRIVERS];    // CAN handler from HAL

    uint8_t _num_drivers;   // Total number of initialized drivers.

    static CANManager *_singleton;  // Object pointer, for singleton checking
};