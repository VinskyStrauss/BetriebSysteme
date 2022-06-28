#ifndef MITARBEITER_H
#define MITARBEITER_H
#include "Anrufer.h"
#include "Warteschlange.h"
#include <mutex>
#include <semaphore.h>
#include <iostream>

using namespace std;

class Mitarbeiter
{
public:
    Mitarbeiter(){

    }

    void takeCall(int& counter, Warteschlange &warteschlange, mutex &m, mutex &msg, sem_t &incomingCall){
        if(warteschlange.empty()){
            m.unlock();
            return;
        }
        Anrufer *kunde = warteschlange.getTop();
        // warteschlange.show();
        kunde->setTimeOut(chrono::system_clock::now());
        kunde->countElapsedSeconds();
        warteschlange.sumDuration();
        warteschlange.pop();
        m.unlock();

        msg.lock();
        cout<<"Accepting call from Anrufer ID "<< kunde->getID() <<endl;
        msg.unlock();
        sleep(5);
        msg.lock();
        cout << "Kunde " << kunde->getID() << " call finished" << endl;
        msg.unlock();
        sem_post(&incomingCall);
        counter++;    
    }    
private:
    
};

#endif
