#ifndef ANRUFER_H
#define ANRUFER_H
#include <unistd.h>
#include "mutex"
#include "semaphore.h"
#include <chrono>

int maxID = 1;

using namespace std; 

class Anrufer
{
public:
    Anrufer(){
        ID = maxID++;
        Callduration = 2;
    }

    int getID(){
        return ID;
    }

    int getDuration(){
        return Callduration;
    }

    // void makeCall(Warteschlange &schlange, mutex &m, sem_t &incomingCall){
    //     if(schlange.check()){
    //         schlange.push(*this);
    //         m.unlock();
    //         sem_wait(&incomingCall);
    //     }else{
    //         m.unlock();
    //         sleep(5);
    //     }
    // }

    void setTimeIn(chrono::time_point<chrono::system_clock> time){
        time_in = time;
    }

    void setTimeOut(chrono::time_point<chrono::system_clock> time){
        time_out = time;
    }

    void countElapsedSeconds(){
        elapsed_seconds = time_out - time_in;
    }

    // auto getTimeIn()const{
    //     return time_in;
    // }

    // auto getTimeOut()const{
    //     return time_out;
    // }

    int  getElapsedSeconds()const{
        return elapsed_seconds.count();
    }

private:
    int ID;
    int Callduration;
    chrono::time_point<chrono::system_clock> time_in, time_out;
    chrono::duration<double> elapsed_seconds;
};

#endif
