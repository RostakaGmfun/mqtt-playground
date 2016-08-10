#include <memory.h>

#include "mqtt.h"

#include <MQTTPacket.h>

struct mqtt_context {
    struct channel channel;
    MQTTTransport mqtt_transport;
    int qos;
    uint8_t packet_buffer;
    size_t packet_buffer_length;
    void *user_data;
    notify_callback on_notify;
};

struct mqtt_context *mqtt_init(int qos, void *user_data)
{
    struct mqtt_context *context = malloc(sizeof(struct mqtt_context));
    if (!context) {
        return NULL;
    }

    context->qos = qos;
    context->user_data = user_data;

    if (channel_init(&context->channel) != 0) {
        free(context);
        return NULL;
    }

    return context;
}

int mqtt_connect(struct mqtt_context *context, const char *host, uint16_t port)
{
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    size_t packet_len = 0;

    if (channel_connect(&context->channel, host, port) != 0) {
        return 1;
    }

    packet_len = MQTTSerialize_connect(context->packet_buffer,
            context->packet_buffer_length, &connect_data);

    if (!packet_len) {
        return 1;
    }

    if (channel_write(&context->channel, context->packet_buffer,
                packet_len) != packet_len) {
        return 1;
    }

    return 0;
}

