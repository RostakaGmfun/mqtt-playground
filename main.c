/**
 * This sample app subscribes to topic a and publishes received messages to b.
 */

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#include "channel.h"

#include <MQTTPacket.h>

static int exit_flag = 0;

static const char *MQTT_HOST = "localhost";
static const uint16_t MQTT_PORT = 1883;
static const size_t PACKET_BUFFER_LEN = 256;

void sigint_handler(int sig)
{
    exit_flag = 1;
}

static int mqtt_connect(struct channel *chan, uint8_t *buf, size_t buf_len)
{
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    size_t packet_len = 0;

    MQTTTransport *mqtt_transport;

    packet_len = MQTTSerialize_connect(buf, buf_len, &connect_data);

    if (!packet_len) {
        return 1;
    }

    if (channel_write(chan, buf, packet_len) != packet_len) {
        return 1;
    }

    if (MQTTPacket_readnb(buf, buf_len, chan->data) != CONNACK) {
        uint8_t connack_rc = 0;
        if (MQTTDeserialize_connack(NULL, &connack_rc, buf, buf_len) != 1 || connack_rc != 0) {
            return 1;
        }
    }

    return 0;
}

static int mqtt_subscribe(struct channel *chan, const char *topic_name,
        uint8_t *buf, size_t buf_len)
{

}

int main()
{
    signal(SIGINT, sigint_handler);

    struct channel chan = { 0 };
    uint8_t packet_buffer[PACKET_BUFFER_LEN];
    MQTTTransport transport_interface;

    if (channel_open(&chan, MQTT_HOST, MQTT_PORT) != 0) {
        fprintf(stderr, "Failed to open TCP connection to MQTT broker\n");
        return EXIT_FAILURE;
    }

    transport_interface.sck = &chan;
    transport_interface.getfn = (int (*)(void *, unsigned char *, int))channel_read;
    chan.data = &transport_interface;

    if (mqtt_connect(&chan, packet_buffer, PACKET_BUFFER_LEN) != 0) {
        fprintf(stderr, "Failed to connect to MQTT broker\n");
        return EXIT_FAILURE;
    }

    printf("Successfully connected to %s:%d\n", MQTT_HOST, MQTT_PORT);

    if (mqtt_subscribe(&chan, "a", packet_buffer, PACKET_BUFFER_LEN) != 0) {
        fprintf(stderr, "Failed to subscribe to topic 'a'\n");
        return EXIT_FAILURE;
    }

    while (exit_flag == 0) {
        // TODO
    }

    channel_close(&chan);

    return EXIT_SUCCESS;
}
