#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
int main () {
    const int N = 3;
    std::cout << "main pid=" << getpid() << std::endl;
    int child_count = 0;
    for (int i=1, j=N; i<=N && j==N; i++ ) {
        pid_t pid1 = fork();
        if(pid1 !=0) { 
            child_count++;
            break;
        }
        else{ 
            child_count = 0;
        }
        std::cout << " i:" << i << " j:" << j << std::endl;
        //std::cout << " parent pid : " << getppid() << std::endl;
        for (int k=1; k<=i && j==N ; k++) {
            pid_t pid= fork();
            
            if ( pid == 0) {
                //std::cout << " parent pid : " << getppid() << std::endl;
                j=0;
                std::cout << "k:" << k << " j:" << j << std::endl;
                child_count = 0;
            }
            else{
                child_count++;
            }
        }
    }
    for(int i = 0; i < child_count ;i++){
        int status;
        int pid = wait(&status);
        std::cout << "detected by parent :  " << getpid() << " end of child pid =" << pid << std::endl;
    }
    std::cout <<" nb children : "<< child_count << std::endl;
    return 0;
}