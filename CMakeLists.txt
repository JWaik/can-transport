add_library(can_transport STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CANManager/Src/CANManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CANManager/Src/CANManager_API.cpp
)

target_include_directories(can_transport PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CANManager/Inc
)

target_link_libraries(can_transport
    stm32cubemx
)
