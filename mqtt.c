#include <stdlib.h>
#include <string.h>

#include "mqtt.h"

#include <MQTTPacket.h>

struct mqtt_context {
    struct channel channel;
    MQTTTransport mqtt_transport;
    uint8_t packet_buffer[MQTT_PACKET_BUFFER_SIZE];
    void *user_data;
    char *client_id;
    notify_callback on_notify;
    int exit_flag;
};

struct mqtt_context *mqtt_init(const char *client_id, void *user_data)
{
    struct mqtt_context *context = malloc(sizeof(struct mqtt_context));
    if (!context) {
        return NULL;
    }

    context->user_data = user_data;
    context->client_id = malloc(strlen(client_id)+1);
    if (!context->client_id) {
        free(context);
        return NULL;
    }
    strcpy(context->client_id, client_id);

    if (channel_init(&context->channel) != 0) {
        free(context);
        return NULL;
    }
    context->mqtt_transport.sck = &context->channel;
    context->mqtt_transport.getfn = channel_getdata;

    return context;
}

void mqtt_destroy(struct mqtt_context *context)
{
    if (context) {
        channel_destroy(&context->channel);
        free(context);
    }
}

int mqtt_connect(struct mqtt_context *context, const char *host, uint16_t port)
{
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    size_t packet_len = 0;

    if (channel_connect(&context->channel, host, port) != 0) {
        return 1;
    }

    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring  = context->client_id;
    connect_data.cleansession = 1;

    packet_len = MQTTSerialize_connect(context->packet_buffer,
            MQTT_PACKET_BUFFER_SIZE, &connect_data);

    if (!packet_len) {
        return 1;
    }

    if (channel_write(&context->channel, context->packet_buffer,
                packet_len) != packet_len) {
        return 1;
    }

    // TODO: timeout
    while (1) {
        if (MQTTPacket_readnb(context->packet_buffer, MQTT_PACKET_BUFFER_SIZE,
                    &context->mqtt_transport) == CONNACK) {

            unsigned char sessionPresent, connack_rc;

            int ret = MQTTDeserialize_connack(&sessionPresent, &connack_rc,
                    context->packet_buffer, MQTT_PACKET_BUFFER_SIZE);
            if (ret!= 1 || connack_rc != 0) {
                // TODO: handle connack_rc
                return 1;
            }
            return 0;
        }
    }
}

void mqtt_set_notification_handler(struct mqtt_context *context, notify_callback handler)
{
    if (context) {
        context->on_notify = handler;
    }
}

int mqtt_subscribe(struct mqtt_context *context, const char *topic_name)
{
    if (!context || !topic_name) {
        return 1;
    }
    // TODO
    return 0;
}

int mqtt_publish(struct mqtt_context *context, const char *topic_name,
        uint8_t *message, size_t message_length, int qos)
{
    if (!context || !topic_name || !message || !message_length) {
        return 1;
    }

    // TODO: add support for QoS 1 and 2
    if (qos != 0) {
        return 1;
    }

    MQTTString tname = MQTTString_initializer;
    // WTF why is it non-const??
    tname.cstring = (char *)topic_name;

    int len = MQTTSerialize_publish(context->packet_buffer, MQTT_PACKET_BUFFER_SIZE,
            0, /* DUP */
            qos,
            0, /* Retain */
            0, /* Packet id */
            tname, message, message_length);

    if (len <= 0) {
        return 1;
    }

    if (channel_write(&context->channel, context->packet_buffer, len) != len) {
        return 1;
    }

    return 0;
}

void mqtt_loop(struct mqtt_context *context)
{
    if (!context) {
        return;
    }

    while (!context->exit_flag) {
        int packet_type = 0;
        while ((packet_type = MQTTPacket_readnb(context->packet_buffer, MQTT_PACKET_BUFFER_SIZE,
                &context->mqtt_transport) == 0));
        if (packet_type < 0) {
            // TODO: error handling
        }

        switch (packet_type) {
            // TODO
        }
    }

}

void mqtt_exit_loop(struct mqtt_context *context)
{
    if (context) {
        context->exit_flag = 1;
    }
}
