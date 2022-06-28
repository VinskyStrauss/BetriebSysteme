#include <iostream>
#include <unistd.h>
#include <thread>
#include <sys/resource.h>
#include <wait.h>

void* threadfunc(void* data){
    int* parentInts = new int[3145728];
    for(int i = 0; i < 3145728; i++){
        parentInts[i] = INT32_MAX;
        usleep(1);
    }

    for(int i = INT32_MAX; i > 0; i--){
        int a = i % 7;
    }

    return NULL;
}

void aufgabe(){

    std::cout << "Starting in ..\n";
    for(int i = 5; i > 0; i--){
        std::cout << i << "\n";
        sleep(1);
    }
    std::cout << "GO\n";
    sleep(1);

    while (true)
    {
        pthread_t mythread;
        pthread_create(&mythread, NULL, threadfunc, NULL);
        usleep(62500);
    }

}

int main(){
    
    aufgabe();
    
    return 0;
}