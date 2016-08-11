#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdint.h>
#include <stddef.h>

struct channel {
    int sockfd; /*!< Socket descriptor assiciated with channel */
};

/**
 * Initializes channel interface
 *
 * @param chan  Pointer to uninitialized channel structure.
 *
 * @return The status code.
 * @retval 0 Success.
 * @retval 1 Error.
 */
int channel_init(struct channel *chan);


/**
 * Opens TCP connection to @host at @port.
 *
 * @param host  String representation of host (resolved by getaddrbyname).
 * @param port  Port number.
 *
 * @return The status code.
 * @retval 0 Success.
 * @retval 1 Error.
 */
int channel_connect(struct channel *chan, const char *host, uint16_t port);


/**
 * Disconnects and destroys channel interface.
 *
 * @param chan  A valid pointer to channel structure.
 *
 * Note that chan should be freed by the caller.
 */
void channel_destroy(struct channel *chan);


/**
 * Writes data to the channel.
 *
 * @param chan      A valid pointer to channel structure.
 * @param buffer    A pointer to read-only buffer containing data to be written.
 * @param size      Number of bytes to write.
 *
 * Note that tcp connection should be opened prior to this call.
 *
 * @return number of bytes written or 0 on error.
 */
size_t channel_write(struct channel *chan, const void *buffer, size_t size);


/**
 * Reads data from the channel.
 *
 * @param chan      A valid pointer to channel structure.
 * @param buffer    A pointer to buffer where the data should be red in.
 * @param size      Number of bytes to read.
 *
 * Note that tcp connection should be opened prior to this call.
 *
 * @return number of bytes written or 0 on error.
 */
size_t channel_read(struct channel *chan, void *buffer, size_t size);


/**
 * Callback used by MQTTPacket library. Basically the same as channel_read.
 *
 * @param sck       A pointer to channel structure.
 * @param buffer    A buffer whre the data should be places.
 * @param size      Number of byrtes to read.
 *
 * @return Number of bytes read.
 * @retval 0 Call again.
 * @retval -1 Error.
 */
int channel_getdata(void *sck, uint8_t *buffer, int size);

#endif // CHANNEL_H
