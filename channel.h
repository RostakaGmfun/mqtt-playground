#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdint.h>
#include <stddef.h>

struct channel {
    int sockfd;
    void *data;
};

int channel_init(struct channel *chan);

int channel_connect(struct channel *chan, const char *host, uint16_t port);

void channel_destroy(struct channel *chan);

size_t channel_write(struct channel *chan, const void *buffer, size_t size);

size_t channel_read(struct channel *chan, void *buffer, size_t size);

int channel_getdata(void *sck, uint8_t *buffer, int size);

#endif // CHANNEL_H
