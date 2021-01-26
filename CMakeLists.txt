cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_PROGRAM_PATH C:/tools/xpack-riscv/bin) #[[todo adjust for a real system]]

# specify cross compilers and tools
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_C_COMPILER riscv-none-embed-gcc)
set(CMAKE_CXX_COMPILER riscv-none-embed-g++)
set(CMAKE_ASM_COMPILER riscv-none-embed-gcc)
set(CMAKE_AR riscv-none-embed-ar)
find_program(CMAKE_OBJCOPY riscv-none-embed-objcopy)
find_program(CMAKE_OBJDUMP riscv-none-embed-objdump)
find_program(SIZE riscv-none-embed-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(rv-longan-nano-clion-ota-demo C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

set(SYSTEM_CLOCK 8000000U)
set(FIRMWARE_DIR ${CMAKE_SOURCE_DIR}/GD32VF103_Firmware_Library_V1.0.0/Firmware)

add_compile_options(-march=rv32imac -mabi=ilp32 -mcmodel=medlow -Wl,-Bstatic -ffunction-sections -fdata-sections)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g3)
endif ()

include_directories(
    ${CMAKE_SOURCE_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/Inc/lcd
    ${FIRMWARE_DIR}/GD32VF103_standard_peripheral/Include
    ${FIRMWARE_DIR}/GD32VF103_standard_peripheral
    ${FIRMWARE_DIR}/RISCV/drivers
)

add_definitions(-DUSE_STDPERIPH_DRIVER -DHXTAL_VALUE=${SYSTEM_CLOCK})

file(GLOB_RECURSE SOURCES
        "${FIRMWARE_DIR}/GD32VF103_standard_peripheral/Source/*.*"
        "${FIRMWARE_DIR}/GD32VF103_standard_peripheral/*.*"
        "${FIRMWARE_DIR}/RISCV/stubs/*.*"
        "${FIRMWARE_DIR}/RISCV/drivers/*.*"
        "${CMAKE_SOURCE_DIR}/Src/*.*"
        "${FIRMWARE_DIR}/RISCV/env_Eclipse/start.s"
        "${FIRMWARE_DIR}/RISCV/env_Eclipse/entry.s"
        )

set(LINKER_SCRIPT ${FIRMWARE_DIR}/RISCV/env_Eclipse/GD32VF103x8.lds)

add_link_options(-march=rv32imac -mabi=ilp32 -mcmodel=medlow -Wl,-Bstatic -Wl,--cref -Wl,--no-relax -nostartfiles)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map  --specs=nosys.specs --specs=nano.specs )
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")