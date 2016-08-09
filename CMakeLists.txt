cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(mqtt-client LANGUAGES C)

include(CMake/Paho.cmake)

add_executable(mqtt_client main.c channel.c)
target_link_libraries(mqtt_client mqtt_packet)
