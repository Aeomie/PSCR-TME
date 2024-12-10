// C++ program to illustrate the client application in the
// socket programming
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h> // For sockaddr_in and related structures
#include <arpa/inet.h>  // For inet_pton and inet_ntop

#define WRITE_PORT 3000
#define READ_PORT 4000
#define BUFFERSIZE 256

void write_message(int socket_fd,const char *server_ip, const char *message) {
    // specifying address
    sockaddr_in serverWrite;
    serverWrite.sin_family = AF_INET;
    serverWrite.sin_port = htons(WRITE_PORT);

    if (inet_pton(AF_INET, server_ip, &serverWrite.sin_addr) <= 0) { // to put the ip address of the server in serverWrite.sin_addr
        perror("Invalid address");
        exit(1);
    }

    if (connect(socket_fd, (struct sockaddr *)&serverWrite, sizeof(serverWrite)) == -1) { // connect to the server in the socket
        perror("Connection failed");
        exit(1);
    }

    send(socket_fd, message, strlen(message) + 1, 0); // send the data , we do +1 to count the size of '\0' inside of it
    printf("Message sent to server: %s\n", message);

    close(socket_fd);
}

void read_message(int socket_fd,const char *server_ip) {

    // specifying address
    sockaddr_in serverRead;
    serverRead.sin_family = AF_INET;
    serverRead.sin_port = htons(READ_PORT);

    if (inet_pton(AF_INET, server_ip, &serverRead.sin_addr) <= 0) { // ip address in serverRead
        perror("Invalid address");
        exit(1);
    }

    if (connect(socket_fd, (struct sockaddr *)&serverRead, sizeof(serverRead)) == -1) { // connect to server
        perror("Connection failed");
        exit(1);
    }

    char message[BUFFERSIZE] = {0};
    recv(socket_fd, message, BUFFERSIZE, 0);
    printf("Message received from server: %s\n", message);

    close(socket_fd);
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Usage: <mode> <server_ip> [message]" << std::endl;
        std::cout << " mode: 'write' to send, 'read' to receive "<< std::endl;
        return 1;
    }
    const char *mode = argv[1];
    std::cout << mode << std::endl;
    const char *server_ip = argv[2];
    std::cout << server_ip << std::endl;
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1){
        perror("Socket creation Failed");
        exit(1);
    }
    if (strcmp(mode, "write") == 0) {
        if (argc != 4) {
            std::cout << "Usage: <mode> <server_ip> [message]" << std::endl;
            return 1;
        }
        write_message(socket_fd,server_ip, argv[3]);
    } else if (strcmp(mode, "read") == 0) {
        read_message(socket_fd,server_ip);
    } else {
        fprintf(stderr, "Invalid mode. Use 'write' or 'read'.\n");
        return 1;
    }


    return 0;
}