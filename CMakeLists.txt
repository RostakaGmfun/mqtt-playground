cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(mqtt-playground LANGUAGES C)

include(CMake/Paho.cmake)

add_executable(mqtt_echo mqtt_echo.c channel.c mqtt.c)
target_link_libraries(mqtt_echo paho_mqtt_packet)
