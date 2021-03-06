/**
 * This sample app subscribes to topic a and publishes received messages to b.
 */

#include <stdlib.h>
#include <signal.h>
#include <stdio.h>

#include "channel.h"
#include "mqtt.h"

static int exit_flag = 0;

static const char *MQTT_HOST = "localhost";
static const uint16_t MQTT_PORT = 1883;
static const size_t PACKET_BUFFER_LEN = 256;

static void on_notify(struct mqtt_context *context, const char *topic,
        uint8_t *message, size_t message_length)
{
    printf("Received message on topic 'a'\n");

    if (mqtt_publish(context, "b", message, message_length, 0) != 0) {
        fprintf(stderr, "Failed to publish message to topic 'b'\n");
    }
}

int main()
{
    struct mqtt_context *mqtt_context = mqtt_init("client", NULL);
    if (!mqtt_context) {
        fprintf(stderr, "Failed to initialize MQTT context\n");
        goto error;
    }

    if (mqtt_connect(mqtt_context, MQTT_HOST, MQTT_PORT) != 0) {
        fprintf(stderr, "Failed to connect to MQTT broker\n");
        goto error;
    }

    printf("Successfully connected to MQTT broker\n");

    mqtt_set_notification_handler(mqtt_context, on_notify);

    if (mqtt_subscribe(mqtt_context, "a") != 0) {
        fprintf(stderr, "Failed to subcribe to topic 'a'\n");
        goto error;
    }

    printf("Successfully subscribed to topic 'a'\n");

    const uint8_t message[] = "Hello, MQTT!";
    if (mqtt_publish(mqtt_context, "a", message, sizeof(message)+1, 0)) {
        fprintf(stderr, "Failed to publish message\n");
        goto error;
    }

    mqtt_loop(mqtt_context);

    return EXIT_SUCCESS;

error:
    mqtt_destroy(mqtt_context);
    return EXIT_FAILURE;

}
