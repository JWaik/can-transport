/**
 * @file    CANManager.cpp
 * @brief   CAN Manager module for managing driver
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Jatuporn Waikoonvet
 */

#include "CANManager.h"


CANManager* CANManager::_singleton;

CANManager::CANManager()
{
    for (uint8_t i = 0; i < MAX_CAN_DRIVERS; i++) {
        _drivers[i] = nullptr;
        _driver_type_cache[i] = Protocol::None;
        _hcan[i] = nullptr;
    }
    _singleton = this;
}

void CANManager::init(void)
{
    // TODO: get semaphore
    // Noted
    // If we use c++ semaphore class like in Ardupilot
    //         1.just construct semaphore object within func -> it will take-blocking
    //         2.After we exit the functoin that sem is called -> sem object destructed! -> give semaphore
    // It's convenience

    _num_drivers = 0;
    
    // TODO:: logging

    // for iterating to construct driver
    Protocol drv_type[MAX_CAN_DRIVERS] = {};
    for (uint8_t i = 0; i < MAX_CAN_IFACES; i++) {
        //todo: get driver config from user 1,2,3...
        uint8_t drv_num = 1;
        if (drv_num == 0 || drv_num > MAX_CAN_DRIVERS) {
            continue;
        }
        drv_num--;

        // if hal empty -> return ?? pass hal from main? TODO: make decision bout this
        // if (_hcan[i] == nullptr) {
        //     continue;
        // }

        // todo: use parameters, or create auto-adjust
        _hcan[i]->Instance = CAN1;
        _hcan[i]->Init.Prescaler = 3;
        _hcan[i]->Init.Mode = CAN_MODE_NORMAL;
        _hcan[i]->Init.SyncJumpWidth = CAN_SJW_1TQ;
        _hcan[i]->Init.TimeSeg1 = CAN_BS1_13TQ;
        _hcan[i]->Init.TimeSeg2 = CAN_BS2_2TQ;
        _hcan[i]->Init.TimeTriggeredMode = DISABLE;
        _hcan[i]->Init.AutoBusOff = DISABLE;
        _hcan[i]->Init.AutoWakeUp = DISABLE;
        _hcan[i]->Init.AutoRetransmission = DISABLE;
        _hcan[i]->Init.ReceiveFifoLocked = DISABLE;
        _hcan[i]->Init.TransmitFifoPriority = DISABLE;

        if (HAL_CAN_Init(_hcan[i]) != HAL_OK) {
            // todo: implement error handler
            // disable irq in CPSR register before stucking in while loop
            // Error_Handler();
            continue;
        }

        if (_drivers[drv_num] != nullptr) {
            //We already initialised the driver just add interface and move on
            _drivers[drv_num]->add_interface(_hcan[i]);
            continue;
        }

        if (_num_drivers >= MAX_CAN_DRIVERS) {
            // We are exceeding number of drivers,
            // Error
        }

        // TODO get driver type from USER Config when build
        // drv_type[i] = ...
        switch (drv_type[i]) {
        case Protocol::DroneCAN:
            // TODO: _drivers[MAX_CAN_DRIVERS] = .... construct DroneCAN lib
            break;
        case Protocol::MultiCAN:
            // TODO: _drivers[MAX_CAN_DRIVERS] = .... construct MultiCAN lib
            break;
        default:
            continue;
        }

        _num_drivers++;
        _drivers[drv_num]->add_interface(_hcan[i]);
    }

    for (uint8_t drv_num = 0; drv_num < MAX_CAN_DRIVERS; drv_num++) {
        //initialise all the Drivers

        // Cache the driver type.
        _driver_type_cache[drv_num] = drv_type[drv_num];

        if (_drivers[drv_num] == nullptr) {
            continue;
        }

        _drivers[drv_num]->init(drv_num);
    }
}

extern "C"
{
void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (canHandle->Instance == CAN1)
    {
        /* CAN clock enable */
        __HAL_RCC_CAN1_CLK_ENABLE();

        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**CAN GPIO Configuration
        PD0     ------> CAN1_RX
        PD1     ------> CAN1_TX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* interrupt Init */
        HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *canHandle)
{
    if (canHandle->Instance == CAN1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        /**CAN GPIO Configuration
        PD0     ------> CAN1_RX
        PD1     ------> CAN1_TX
        */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1);

        /* Interrupt Deinit */
        HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    }
}
}