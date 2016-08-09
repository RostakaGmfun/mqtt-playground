#include "channel.h"

#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int channel_open(struct channel *chan, const char *host, uint16_t port)
{

    struct sockaddr_in addr;
    int sockfd = 0;
    struct timeval tv;

    if (!chan || !host) {
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        return 1;
    }

    struct hostent *host_addr = gethostbyname(host);
    if (!host_addr) {
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr, host_addr->h_addr_list[0], host_addr->h_length);

    if (connect(sockfd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) {
        return 1;
    }

    chan->sockfd = sockfd;

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if (setsockopt(chan->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) != 0) {
        return 1;
    }

    return 0;
}

void channel_close(struct channel *chan)
{
    if(chan) {
        close(chan->sockfd);
    }
}

size_t channel_write(struct channel *chan, const void *buffer, size_t size)
{
    if (!chan || !buffer || !size) {
        return 0;
    }

    return write(chan->sockfd, buffer, size);
}

size_t channel_read(struct channel *chan, void *buffer, size_t size)
{
    if (!chan || !buffer || !size) {
        return 0;
    }

    size_t ret = read(chan->sockfd, buffer, size);

    printf("channel_read, size: %zu, ret: %zu\n", size, ret);
    return ret;
}

int channel_getdata(void *sck, unsigned char *buffer, int size)
{
    struct channel *chan = sck;

    ssize_t ret = read(chan->sockfd, buffer, size);
    if (ret <= 0) {
        //connection has been closed or error occured
        return -1;
    }
}
