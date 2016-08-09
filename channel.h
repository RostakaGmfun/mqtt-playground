#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdint.h>
#include <stddef.h>

struct channel {
    int sockfd;
    void *data;
};

int channel_open(struct channel *chan, const char *host, uint16_t port);

void channel_close(struct channel *chan);

size_t channel_write(struct channel *chan, const void *buffer, size_t size);

size_t channel_read(struct channel *chan, void *buffer, size_t size);

#endif // CHANNEL_H
