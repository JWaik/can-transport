#pragma once

#define ENABLED 1
#define DISABLED 0

#ifndef CANMGR_ENABLED
#define CANMGR_ENABLED ENABLED
#endif

#ifndef CANMGR_CUBEMX_CAN_ENABLED
#define CANMGR_CUBEMX_CAN_ENABLED ENABLED
#endif

#define CANMGR_MAX_CAN_DRIVERS 2
#define CANMGR_MAX_CAN_IFACES 2

#define CANMGR_CAN1_IDX 0
#define CANMGR_CAN2_IDX 1

#define CANMGR_CAN1_DRIVER_TYPE     0
#define CANMGR_CAN2_DRIVER_TYPE     0

// GPIO Settings
#define CANMGR_CAN1_GPIO_PORT    GPIOD                       // GPIO Port
#define CANMGR_CAN1_GPIO_PIN_TX  GPIO_PIN_1                  // CAN Tx
#define CANMGR_CAN1_GPIO_PIN_RX  GPIO_PIN_0                  // CAN Rx

#define CANMGR_GPIO_PULL    GPIO_NOPULL                 // GPIO Pull mode
#define CANMGR_GPIO_SPEED   GPIO_SPEED_FREQ_VERY_HIGH   // Pin speed, High speed 
#define CANMGR_GPIO_MODE    GPIO_MODE_AF_PP             // Use as alternate function
#define CANMGR_GPIO_AF_MAP  GPIO_AF9_CAN1               // Alternate function map to CAN Peripheral

