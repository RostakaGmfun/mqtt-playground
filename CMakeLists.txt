cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(mqtt-playground LANGUAGES C)

include(CMake/Paho.cmake)

if(NOT DEFINED MQTT_PACKET_BUFFER_SIZE)
    set(MQTT_PACKET_BUFFER_SIZE 256)
endif(NOT DEFINED MQTT_PACKET_BUFFER_SIZE)

add_executable(mqtt_echo mqtt_echo.c channel.c mqtt.c)
target_link_libraries(mqtt_echo paho_mqtt_packet)
target_compile_definitions(mqtt_echo PRIVATE -DMQTT_PACKET_BUFFER_SIZE=${MQTT_PACKET_BUFFER_SIZE})
