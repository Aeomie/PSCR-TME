#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define WRITE_PORT 3000
#define READ_PORT 4000
#define BUFFERSIZE 256
int read_count;
bool reading_only;
bool close_server;
char buffer[BUFFERSIZE] = { 0 };


void handle_write(int write_socket) {
    if(!reading_only){
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sock = accept(write_socket, nullptr, nullptr); // second parameter to get Client information , ip & stuff , third parameter i dont know
        if (client_sock == -1) {
            perror("Failed to accept write connection");
            return;
        }

        printf("Write connection accepted.\n");

        ssize_t bytes_received = recv(client_sock, buffer, BUFFERSIZE, 0);
        if (bytes_received == -1) {
            perror("Failed to receive message");
        } else {
            buffer[BUFFERSIZE - 1] = '\0';
            printf("Message received: %s\n",buffer);
        }
        if (strcmp("close", buffer) == 0) {
            close_server = true;
        }
        reading_only = true;
        read_count = 0;
        close(client_sock);
    }
}

void handle_read(int read_socket, int N) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);

        int client_sock = accept(read_socket, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock == -1) {
            perror("Failed to accept read connection");
            return;
        }
        if(read_count < N){
        ssize_t bytes_sent = send(client_sock, buffer, strlen(buffer) + 1, 0);
        if (bytes_sent == -1) {
            perror("Failed to send message");
        } else {
            printf("Message sent to client.\n");
        }
        read_count++;
        if(read_count >= N){
            reading_only = false;
        }
        close(client_sock);
        }
}

int main(int argc, char *argv[]){
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        std::cout << "Usage : N \n" << std::endl;
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        std::cout << "N must be pos value & Integer \n" << std::endl;
        return 1;
    }

    //creating sockets
    int writing_socket = socket(AF_INET, SOCK_STREAM, 0);
    int reading_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (writing_socket == -1 || reading_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }
    // specifiying writing address
    sockaddr_in writingAdd;
    writingAdd.sin_family = AF_INET;
    writingAdd.sin_port = htons(WRITE_PORT);
    writingAdd.sin_addr.s_addr = INADDR_ANY;

    // binding socket
       bind(writing_socket, (struct sockaddr*)&writingAdd,
         sizeof(writingAdd));

    if (listen(writing_socket, 5) == -1) {
        perror("Listen failed for write socket");
        return 1;
    }

    close_server = false;
    // specifiying reading address
    sockaddr_in readingAdd;
    readingAdd.sin_family = AF_INET;
    readingAdd.sin_port = htons(READ_PORT);
    readingAdd.sin_addr.s_addr = INADDR_ANY;

    // binding socket
       bind(reading_socket, (struct sockaddr*)&readingAdd,
         sizeof(readingAdd));

    // listening to the assigned socket
    if (listen(reading_socket, 5) == -1) {
        perror("Listen failed for read socket");
        return 1;
    }


    printf("Server is running:\n");
    printf("  - Write Port: %d\n", WRITE_PORT);
    printf("  - Read Port: %d\n", READ_PORT);
    printf("  - N: %d\n", N);
    printf("  - Read_count Init: %d\n", read_count);
    while(!close_server){
        if(!reading_only){
            handle_write(writing_socket);
        }
        handle_read(reading_socket, N);
    }

    close(writing_socket);
    close(reading_socket);
}