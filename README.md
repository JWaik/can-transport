# CAN Transport

A C++ CAN transport layer supporting multiple CAN backends for STM32 embedded and real-time systems.

The library focuses on **transport-level abstraction**, separating CAN messaging logic from hardware- or vendor-specific drivers.  
This allows the same application or protocol stack to run on multiple CAN backends.

## Key Goals
- Provide a C++ transport abstraction for CAN communication
- Support multiple CAN types (Classic CAN, vendor-specific CAN)
- Allow pluggable CAN backends without changing application code
- Be suitable for protocol stacks such as DroneCAN / UAVCAN and custom CAN-based protocols
