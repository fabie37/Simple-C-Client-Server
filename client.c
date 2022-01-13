#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void dnsLookup(const char *url, const char *port, struct addrinfo **addresses);

int main() {
    // 1. Set up
    printf("Initialising...\n");

    // 2. Find tcp server via dns lookup
    const char *url = "www.catinella.co.uk";
    const char *port = "2000";
    struct addrinfo *addresses;
    dnsLookup(url, port, &addresses);
    printf("Trying connection...\n");

    // 3. Iteration through possible protocals
    struct addrinfo *addr;
    int fd;

    for (addr = addresses; addr != NULL; addr = addr->ai_next) {
        fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

        if (fd == -1) {
            perror("Couldn't connect to this socket, trying another");
            continue;
        }

        if (connect(fd, addr->ai_addr, addr->ai_addrlen) == -1) {
            printf("Couldn't connect \n");
            close(fd);
            continue;
        }
        break;
    }
    if (addr == NULL) {
        printf("Couldn't connect to url.\n");
        exit(0);
    } else {
        printf("Successfully connected to server.\n");
    }

    // 4. Send data to Server
    char *data = "Hello world!";
    size_t data_len = strlen(data) * sizeof(char);
    int flags = MSG_NOSIGNAL;

    if (send(fd, data, data_len, flags) == -1) {
        perror("Failed to send data");
    } else {
        printf("Hurray I sent some data!");
    }
    close(fd);
}

void dnsLookup(const char *url, const char *port, struct addrinfo **addresses) {
    struct addrinfo hints;
    int i;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;      // Unspecified protocol
    hints.ai_socktype = SOCK_STREAM;  // Want a tcp socket

    // Do DNS lookup, addressess is a pointer head to a linked list
    if ((i = getaddrinfo(url, port, &hints, addresses)) != 0) {
        printf("Error: Unable to lookup IP address: %s", gai_strerror(i));
    }
}