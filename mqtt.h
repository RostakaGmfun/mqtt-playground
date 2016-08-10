#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>
#include <stddef.h>

#include "channel.h"

struct mqtt_context;

typedef int (*notify_callback)(struct mqtt_context *context, const char *topic,
        uint8_t *message, size_t message_length);

struct mqtt_context *mqtt_init(const char *client_id, void *user_data);

void mqtt_destroy(struct mqtt_context *context);

int mqtt_connect(struct mqtt_context *context, const char *host, uint16_t port);

void mqtt_set_notification_handler(struct mqtt_context *context, notify_callback handler);

int mqtt_subscribe(struct mqtt_context *context, const char *topic_name);

int mqtt_publish(struct mqtt_context *context, const char *topic_name,
        uint8_t *message, size_t mesasge_length, int qos);

void mqtt_loop(struct mqtt_context *context);

void mqtt_exit_loop(struct mqtt_context *context);

#endif // MQTT_H
