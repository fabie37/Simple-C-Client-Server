#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFLEN 1500

int main() {
    // 1. Set up
    printf("Initialising...\n");
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1) {
        perror("Something went wrong with the init:");
        exit(0);
    }

    // 2. Binding to ip and port
    struct sockaddr_in addr = {
        .sin_family = AF_INET,          // IPV4
        .sin_addr.s_addr = INADDR_ANY,  // Any avail network interface
        .sin_port = htons(2000)         // Port
    };

    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to bind ip and port");
    }

    // 3. Start listening
    int backlog = 10;

    if (listen(fd, backlog) == -1) {
        perror("Server couldn't listen.");
    }
    printf("Server listening...\n");

    // 4. Accept connections
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t cliaddr_len = sizeof(cliaddr);

        // this is the file descriptor of the client
        int connfd = accept(fd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if (connfd == -1) {
            perror("Failed to connect with incomming client.");
        }

	printf("Accepted a connection!\n");

        // 5. Read Data from client
        ssize_t rcount;
	int recvCount = 0;
        char buf[BUFLEN];
	memset(buf, 0, BUFLEN);
        int flags = 0;  // No flags set

	
        rcount = recv(connfd, buf, BUFLEN, flags);
	while (rcount > 0) {
		printf("Count: %ld\n", rcount);
		recvCount += rcount;
		rcount = recv(connfd, buf+recvCount, BUFLEN,flags);
	}
        if (rcount == -1) {
            printf("Failed to read data from client.\n");
        }
	printf("Got something!\n");
        for (int i=0; i<BUFLEN; i++) {
		printf("%c",buf[i]);
	}
	printf("\nEnd of message\n");
        close(connfd);
    }
}
