#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>
#include <stddef.h>

#include "channel.h"

struct mqtt_context;

typedef int (*notify_callback)(struct mqtt_context *cotnext, const char *topic);

struct mqtt_context *mqtt_init(int qos, void *user_data);

void mqtt_destroy(struct mqtt_context *cotnext);

int mqtt_connect(struct mqtt_context *context, const char *host, uint16_t port);

int mqtt_subscribe(struct mqtt_context *context, const char *topic_name);

void mqtt_loop(struct mqtt_context *context);

#endif // MQTT_H
