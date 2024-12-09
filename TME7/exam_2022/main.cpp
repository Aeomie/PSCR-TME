#include <iostream>
#include <cstring>
#include <thread>
#include <vector>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <csignal>

#include "bufferNreadproc.h"
int main(int argc, char *argv[])
{
    const int N = atoi(argv[1]);
    size_t shmsize = sizeof(BufferNReadProc);

    const char* buff_name = "/mybuffer";
    shm_unlink(buff_name); // incase it crashed before
    BufferNReadProc::createBuffer(buff_name, N);

    while (true){
        usleep(100000); // sleep every 0.1 sec
        BufferNReadProc *buffer = BufferNReadProc::openBuffer(buff_name);
        time_t now = time(nullptr);
            if((now - buffer->get_last_activity()) >= 20){
                // return false and get out of loop
                std::cout << "time now : " << now << std::endl;; 
                std::cout << "time last updated:" << buffer->get_last_activity() << std::endl;
                std::cout << "time that went by : "<< (now - buffer->get_last_activity()) << std::endl;
                std::cout << " Buffer closing" << std::endl;;
                // CLOSING BUFFER UNTIL USE
                buffer->~BufferNReadProc();
                buffer->closeBuffer(buffer);
                buffer->deleteBuffer(buff_name);

                break;
            }
        buffer->~BufferNReadProc();
    }

    return 0;
}


  