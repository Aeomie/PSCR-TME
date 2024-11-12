#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <time.h>
int HP = 5;
std::string name;




void randsleep() {
	int r = rand();
	double ratio = (double)r / (double) RAND_MAX;
	struct timespec tosleep;
	tosleep.tv_sec =0;
	// 300 millions de ns = 0.3 secondes
	tosleep.tv_nsec = 300000000 + ratio*700000000;
	struct timespec remain;
	while ( nanosleep(&tosleep, &remain) != 0) {
		tosleep = remain;
	}
}

void handler(){
    std::cout << "cout paré" << std::endl;
}
void decrement_hp(int sig){
    HP--;
    std::cout << name <<" hp left : " << HP << std::endl;

    if(HP <= 0){
        std::cout << "YOU DIED : " << name << " and pid : " <<getpid() << std::endl;
        exit(1);
    }
}
void attaque(pid_t adversary){
    struct sigaction act;
    sigfillset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = &decrement_hp;
    sigaction(SIGINT, &act, NULL);
    
    if(kill(adversary, SIGINT) == -1) {
        std::cout<< "PLEASE STOP , Opponent is already dead" << std::endl;
        exit(0);    
    }
    randsleep(); // should be random
}


void counter_handler(int sig){
    std::cout << "cout paré" << std::endl;
}
void defense(){
    if(name == "Luke"){
        struct sigaction act;
        sigfillset(&act.sa_mask);
        act.sa_flags = 0;
        act.sa_handler = counter_handler;
        sigaction(SIGINT, &act, NULL);

        sigset_t mask, oldmask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        
        // Block SIGINT temporarily
        sigprocmask(SIG_BLOCK, &mask, &oldmask);

        randsleep(); // Random sleep while signal is blocked

        // Use sigsuspend to wait for SIGINT
        sigsuspend(&oldmask);

        sigprocmask(SIG_SETMASK, &oldmask, NULL);

    }
    else{
        struct sigaction act;
        sigfillset(&act.sa_mask);
        act.sa_flags = 0;
        act.sa_handler = SIG_IGN;
        sigaction(SIGINT, &act, NULL);

        randsleep(); // random sleep time

    }
}

void combat(pid_t adversary){
    while(true){
        defense();
        attaque(adversary);
    }
}


pid_t wait_till_pid(pid_t pid) {
    while (true) {
        pid_t p = wait(nullptr);
        if (p == -1 || p == pid) {
            return p;
        }
    }
}

bool timed_out(false);  // Atomic flag to indicate if the timeout occurred

void alarm_handler(int sig) {
    if (sig == SIGALRM) {
        timed_out = true;  // Set the flag to indicate timeout
    }
}


// QS7 
int wait_till_pid(pid_t pid, int sec) {
    signal(SIGALRM, alarm_handler);
    // Set the alarm
    alarm(sec);
    // Loop to wait for the specified child process
    while (true) {
        pid_t p = wait(nullptr);

        // If the specific child terminated, return its PID and cancel the alarm
        if (p == pid) {
            alarm(0);  // Cancel the alarm if we exit successfully
            return p;
        }

        // If wait was interrupted, check the timeout flag
        if (timed_out) {
            return 0;  // Return 0 to indicate timeout
        }
    }
}


// QS8
// V2 : signal

pid_t wait_till_pid(pid_t pid, int sec) {
    static bool timeout;
    timeout = false;

    signal(SIGALRM, [](int) { 
        std::cout << "received SIGALRM" << std::endl; 
        timeout = true;
    });
    // lambda function is a need because u wont have access to timeout otherwise
    signal(SIGCHLD, [](int) { 
        std::cout << "received SIGCHLD" << std::endl; 
    });
    
    alarm(sec);

    sigset_t set; // create new set
    sigfillset(&set); // fill it all
    sigdelset(&set, SIGALRM);
    sigdelset(&set, SIGCHLD);
    // remove sigalrm and sigchild so that sigsuspend blocks all except thesee 2
    while (true) {
        std::cout << "waiting..." << std::endl;
        sigsuspend(&set); // suspending all the other signals
        
        if (timeout) {
            std::cout << "Alarm wins" << std::endl;
            return -1;
        } else {
            pid_t p = wait(nullptr);
            std::cout << "wait answered " << p << std::endl;
            
            if (p == pid) {
                alarm(0);
            }
            if (p == -1 || p == pid) {
                return p;
            }
        }
    }
}

int main () {

    pid_t pid = fork();
    if(pid == 0){   
        name = "Luke";
        std::cout << "Son : "<< name << std::endl;
        combat(getppid());
    }
    else{
        name = "Vador";
        std::cout << "Father : "<< name << std::endl;
        combat(pid);       
    }
return 0;
}