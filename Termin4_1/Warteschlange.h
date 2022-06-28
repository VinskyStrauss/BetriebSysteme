#ifndef WARTESCHLANGE_H
#define WARTESCHLANGE_H
#include "Anrufer.h"
#include <stack>
#include <iostream>
#include <chrono>

using namespace std;

auto total_duration = 0;

class Warteschlange
{
public:
    Warteschlange(){
        
    }

    bool check(){
        if(schlange.size()<15){
            return true;
        }
        return false;
    }

    void push(Anrufer *anrufer){
        schlange.push_back(anrufer);
    }

    void pop(){
        if(mode == 1)
            schlange.pop_front();
        else if(mode == 2)
            schlange.pop_back();
            
    }

    Anrufer* getTop(){
        if(mode == 1)
            return schlange[0];
        else if(mode == 2)
            return schlange[schlange.size()-1];
        return nullptr;
    }

    bool empty(){
        if(schlange.size() == 0){
            return true;
        }
        return false;
    }

    bool addCaller(Anrufer *kunde, mutex &m, sem_t &incomingCall){
        if(this->check()){
            schlange.push_back(kunde);
            kunde->setTimeIn(chrono::system_clock::now());
            m.unlock();
            sem_wait(&incomingCall);
            return true;
        }
        m.unlock();
        sleep(1);
        return false;
        
    }

    void show(){
        cout << "Schlange : " ;
        for(int i = 0; i < schlange.size(); i++){
            cout << schlange[i]->getID() << " "; 
        }
        cout << endl;
    }

    bool isInSchlange(Anrufer* anrufer){
        for(int i = 0; i < schlange.size(); i++){
            if(schlange[i] == anrufer) return true; 
        }
        return false;
    }

    long getSecond() const{
        return second;
    }

    void incrementSecond(){
        second++;
    }

    void sumDuration(){
        int duration = getTop()->getElapsedSeconds();
        total_duration += duration;
    }

    void showAverageDuration(int NUM){
        cout << "average duration " << total_duration/NUM << " second(s)" << endl;
    }

    void setMode(int input){
        mode = input;
    }
private:
    deque<Anrufer*> schlange;
    long second = 0;
    int mode;
};

#endif
