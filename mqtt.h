#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>
#include <stddef.h>

#include "channel.h"

/**
 * Represents context for all MQTT operations.
 */
struct mqtt_context;


/**
 * Called when messages from subscribed topics are received.
 *
 * @param context           A pointer to valid mqtt_context structure.
 * @param topic_name        String representation of topic name.
 * @param message           Pointer to the start of message, represented as a sequence of bytes.
 * @param message_length    Length of message in bytes.
 */
typedef void (*notify_callback)(struct mqtt_context *context, const char *topic_name,
        uint8_t *message, size_t message_length);


/**
 * Initialize mqtt context.
 *
 * This involves initialization of TCP channel.
 *
 * @param client_id     A string representation of client ID used throughout MQTT protocol.
 * @param user_data     A pointer to user context (used in ntify_callback, @see mqtt_get_user_data)
 *
 * @return A pointer to initialized mqtt_context structure or NULL on error.
 */
struct mqtt_context *mqtt_init(const char *client_id, void *user_data);


/**
 * Destroy MQTT context.
 *
 * @param context   A pointer to valid mqtt_context structure.
 */
void mqtt_destroy(struct mqtt_context *context);


/**
 * Attempt to connect to MQTT broker.
 *
 * @param context       A valid pointer to initialized mqtt_context structure.
 * @param host          A string representation of broker's host.
 * @param port          Broker's port number.
 *
 * @return Status code.
 * @retval 0    Success.
 * @retval 1    Error.
 */
int mqtt_connect(struct mqtt_context *context, const char *host, uint16_t port);


/**
 * Specify notofication handler for MQTT context.
 *
 * @param context   A pointer to initialized mqtt_context structure.
 * @param handler   A pointer to handler function.
 */
void mqtt_set_notification_handler(struct mqtt_context *context, notify_callback handler);


/**
 * Subscribe to topic.
 *
 * @param context       A pointer to initialized mqtt_context structure.
 * @param topic_name    A string representation of topic name.
 *
 * @retern Status code
 * @retval 0 Success.
 * @retval 1 Error.
 */
int mqtt_subscribe(struct mqtt_context *context, const char *topic_name);


/**
 * Unsubscribe from topic.
 *
 * @param context       A pointer to initialized mqtt_context structure.
 * @param topic_name    A string representation of topic name.
 *
 * @retern Status code
 * @retval 0 Success.
 * @retval 1 Error.
 */
int mqtt_unsubscribe(struct mqtt_context *context, const char *topic_name);

/**
 * Publish message to topic_name.
 *
 * @param context           A pointer to initialized mqtt_context structure.
 * @param topic_name        A string representation of topic name.
 * @param mesasge           Pointer to buffer to be sent.
 * @param message_length    Length of message in bytes.
 * @param qos               QoS level (0, 1, or 2).
 *
 * @return Status code.
 * @retval 0 Success.
 * @retval 1 Error.
 */
int mqtt_publish(struct mqtt_context *context, const char *topic_name,
        uint8_t *message, size_t mesasge_length, int qos);


/**
 * Enter the main packet processing loop.
 *
 * @param context           A pointer to initialized mqtt_context structure.
 */
void mqtt_loop(struct mqtt_context *context);

/**
 * Shut down the main packet processing loop.
 *
 * @param context           A pointer to initialized mqtt_context structure.
 */
void mqtt_exit_loop(struct mqtt_context *context);


/**
 * Retreive user data from mqtt_context.
 *
 * @see mqtt_init.
 *
 * @param context           A pointer to initialized mqtt_context structure.
 *
 * @return Pointer to user data.
 */
void *mqtt_get_user_data(struct mqtt_context *context);


#endif // MQTT_H
