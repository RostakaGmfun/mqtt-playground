set(PAHO_ROOT ${CMAKE_SOURCE_DIR}/paho.mqtt.embedded-c)
set(PAHO_PACKET_SRC ${PAHO_ROOT}/MQTTPacket/src)

add_library(mqtt_packet ${PAHO_PACKET_SRC}/MQTTConnectClient.c
                        ${PAHO_PACKET_SRC}/MQTTConnectServer.c
                        ${PAHO_PACKET_SRC}/MQTTDeserializePublish.c
                        ${PAHO_PACKET_SRC}/MQTTFormat.c
                        ${PAHO_PACKET_SRC}/MQTTPacket.c
                        ${PAHO_PACKET_SRC}/MQTTSerializePublish.c
                        ${PAHO_PACKET_SRC}/MQTTSubscribeClient.c
                        ${PAHO_PACKET_SRC}/MQTTSubscribeServer.c
                        ${PAHO_PACKET_SRC}/MQTTUnsubscribeClient.c
                        ${PAHO_PACKET_SRC}/MQTTUnsubscribeServer.c)

target_include_directories(mqtt_packet PUBLIC ${PAHO_PACKET_SRC})
