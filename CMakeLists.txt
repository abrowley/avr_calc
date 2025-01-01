cmake_minimum_required(VERSION 3.28)
project(avr_calc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD ${CMAKE_CXX_STANDARD})


set(MCU   atmega328p)
set(F_CPU 16000000UL)
set(BAUD  9600)
add_definitions(-DF_CPU=${F_CPU})

set(PROG_TYPE atmelice_isp)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

# program names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  /usr/local/bin/avrdude -v)


set(CDEBUG    "-gstabs -g2 -ggdb3")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-O0 -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CTUNING}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

set(FUSES -U lfuse:w:0xff:m -U hfuse:w:0xde:m -U efuse:w:0x05:m)

add_executable(${PROJECT_NAME}
        main.cpp
        lib/blink.c
        lib/font.c
        lib/i2c.c
        lib/lcd.c
)
target_include_directories(${PROJECT_NAME} PUBLIC lib)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS ${PROJECT_NAME})
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS ${PROJECT_NAME})

add_custom_target(fuses VERBATIM COMMAND  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${FUSES} DEPENDS hex)
add_custom_target(flash VERBATIM COMMAND  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)