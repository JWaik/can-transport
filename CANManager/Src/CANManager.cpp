/**
 * @file    CANManager.cpp
 * @brief   CAN Manager module for managing driver
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2026 Jatuporn Waikoonvet
 */

#include "CANManager.h"

#if CANMGR_ENABLED

CANManager* CANManager::_singleton;

CANManager::CANManager()
{
    for (uint8_t i = 0; i < CANMGR_MAX_CAN_DRIVERS; i++) {
        _drivers[i] = nullptr;
        _driver_type_cache[i] = Protocol::None;
    }
    _singleton = this;
}

void CANManager::init(CAN_HandleTypeDef *hcan1, CAN_HandleTypeDef *hcan2)
{
    // TODO: get semaphore
    // Noted
    // If we use c++ semaphore class like in Ardupilot
    //         1.just construct semaphore object within func -> it will take-blocking
    //         2.After we exit the functoin that sem is called -> sem object destructed! -> give semaphore
    // It's convenience

    // Get CAN handle from main program
    _hcan[CANMGR_CAN1_IDX] = hcan1;
    _hcan[CANMGR_CAN2_IDX] = hcan2;

    _num_drivers = 0;
    
    // TODO:: logging

    // Set driver type for CAN1, CAN2 drivers
    Protocol drv_type[CANMGR_MAX_CAN_DRIVERS] = {(Protocol)CANMGR_CAN1_DRIVER_TYPE, (Protocol)CANMGR_CAN2_DRIVER_TYPE};

    // for iterating to construct driver
    for (uint8_t i = 0; i < CANMGR_MAX_CAN_IFACES; i++) {

        // CAN handle type is not created or initialized
        if (_hcan[i] == nullptr || _hcan[i]->State == HAL_CAN_STATE_RESET) {
            continue;
        }

#if CANMGR_CUBEMX_CAN_ENABLED == DISABLED
        // todo: use parameters, or create auto-adjust
        // add support can2
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
#endif

        if (_drivers[i] != nullptr) {
            // We already initialised the driver just add interface and move on
            _drivers[i]->add_interface(_hcan[i]);
            continue;
        }

        if (_num_drivers >= CANMGR_MAX_CAN_DRIVERS) {
            // We are exceeding number of drivers,
            // Error
        }

        switch (drv_type[i]) {
        case Protocol::DroneCAN:
            // TODO: _drivers[i] = .... construct DroneCAN lib
            break;
        case Protocol::MultiCAN:
            // TODO: _drivers[i] = .... construct MultiCAN lib
            break;
        default:
            continue;
        }

        _num_drivers++;
        _drivers[i]->add_interface(_hcan[i]);
    }

    for (uint8_t drv_num = 0; drv_num < CANMGR_MAX_CAN_DRIVERS; drv_num++) {
        // initialise all the Drivers
        // Cache the driver type.
        _driver_type_cache[drv_num] = drv_type[drv_num];

        if (_drivers[drv_num] == nullptr) {
            continue;
        }

        _drivers[drv_num]->init(drv_num);
    }
}

void CANManager::loop_can1()
{
    if (_drivers[CANMGR_CAN1_IDX] == nullptr) {
        return;
    }
    // TODO: don't forget to check is_initialized() in driver
    _drivers[CANMGR_CAN1_IDX]->loop();
}

void CANManager::loop_can2()
{
    if (_drivers[CANMGR_CAN2_IDX] == nullptr) {
        return;
    }
    _drivers[CANMGR_CAN2_IDX]->loop();
}

#if CANMGR_CUBEMX_CAN_ENABLED == DISABLED
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

        // TODO: User config pin
        GPIO_InitStruct.Pin = CANMGR_CAN1_GPIO_PIN_RX | CANMGR_CAN1_GPIO_PIN_TX;
        GPIO_InitStruct.Mode = CANMGR_GPIO_MODE;
        GPIO_InitStruct.Pull = CANMGR_GPIO_PULL;
        GPIO_InitStruct.Speed = CANMGR_GPIO_SPEED;
        GPIO_InitStruct.Alternate = CANMGR_GPIO_AF_MAP;
        HAL_GPIO_Init(CANMGR_CAN1_GPIO_PORT, &GPIO_InitStruct);

        /* interrupt Init */
        HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
    }
    else if (canHandle->Instance == CAN2)
    {
        // TODO:
    }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *canHandle)
{
    if (canHandle->Instance == CAN1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_CAN1_CLK_DISABLE();

        // TODO: User Config Pin
        HAL_GPIO_DeInit(CANMGR_CAN1_GPIO_PORT, CANMGR_CAN1_GPIO_PIN_TX | CANMGR_CAN1_GPIO_PIN_RX);

        /* Interrupt Deinit */
        HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
        HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    }
    else if (canHandle->Instance == CAN2)
    {
        // TODO:
    }
}
}
#endif // CANMGR_CUBEMX_CAN_ENABLED
#endif // CANMGR_ENABLED