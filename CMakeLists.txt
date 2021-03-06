CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
SET(CMAKE_SYSTEM_NAME Generic)

#============================================================================================
PROJECT(avr-clion)

SET(DEVICE "atmega256rfr2")
SET(FREQ "16000000")
SET(BAUD "19200")
SET(PROGRAMMER "xplainedpro")


#============================================================================================

SET(CMAKE_C_COMPILER /usr/local/bin/avr-gcc)
SET(CMAKE_CXX_COMPILER /usr/local/bin/avr-g++)

SET(CMAKE_C_FLAGS  "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -DBAUD=${BAUD} -std=gnu99 -Wl,--gc-sections")
SET(CMAKE_CXX_FLAGS "-Os -mmcu=${DEVICE} -DF_CPU=${FREQ}UL -DBAUD=${BAUD} -Wl,--gc-sections")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

INCLUDE_DIRECTORIES("/usr/local/include")

#============================================================================================


SET(SOURCE_FILES
        config/config.c
        oled/oled.c
        rf/rf.c
        usart/usart.c
        main_tx.c
        debug/debug.c
        )

#============================================================================================

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avr-objcopy -O ihex -R.eeprom ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load"  --change-section-lma .eeprom=0 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.eep)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avrdude -p ${DEVICE} -c ${PROGRAMMER} -P usb -U flash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avrdude -p ${DEVICE} -c ${PROGRAMMER} -P usb -v)
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD COMMAND avr-size ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} --mcu=${DEVICE} --format=avr)
