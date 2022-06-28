#include "Anrufer.h"
#include "Warteschlange.h"
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <semaphore.h>
#include "Mitarbeiter.h"
#include <iostream>

using namespace std;

int counter = 0;
int NUM_OF_CALLS = 100;
int NUM_OF_MITARBEITER = 3;
mutex m;
mutex msg;
sem_t incomingCall;

Warteschlange* warteschlange = new Warteschlange();
Mitarbeiter mitarbeiter;

void* takeCallHelper(void* arg){
    while(counter != NUM_OF_CALLS){
        m.lock();
        mitarbeiter.takeCall(counter, *warteschlange, m, msg, incomingCall);
    }

    return NULL;
}

void* makeCallHelper(void* arg){
    Anrufer *anrufer = new Anrufer();
    do{
        m.lock();
    }
    while(!warteschlange->addCaller(anrufer, m, incomingCall));
    
    
    return NULL;
}

void* time_activity(void* arg){
    while(true){
        sleep(1);
        warteschlange->incrementSecond();
    }
    return NULL;
}

void userDialog(){
    int eingabe;
    cout << "(1) FIFO" << endl;
    cout << "(2) LIFO" << endl;
    cout << "input: ";
    cin >> eingabe;
    warteschlange->setMode(eingabe);
}
int main(){
    userDialog();

    sem_init(&incomingCall, 0, 0);
    pthread_t* mitarbeiterT = new pthread_t[NUM_OF_MITARBEITER]();
    pthread_t* anruferT = new pthread_t[NUM_OF_CALLS]();

    for(int i = 0; i < NUM_OF_MITARBEITER; i++){
        long id = pthread_create(&mitarbeiterT[i], NULL, &takeCallHelper, NULL);
        if(id != 0) exit(-1);
    }

    for(int i = 0; i < NUM_OF_CALLS; i++){
        long id = pthread_create(&anruferT[i], NULL, &makeCallHelper, NULL);
        if(id != 0) exit(-1);
    }

    for(int i = 0; i < NUM_OF_MITARBEITER; i++){
        pthread_join(mitarbeiterT[i], NULL);
    }

    for(int i = 0; i < NUM_OF_CALLS; i++){
        pthread_join(anruferT[i], NULL);
        // cout << anruferT[i] << " join " << endl;
    }

    warteschlange->showAverageDuration(NUM_OF_CALLS);
    delete[] mitarbeiterT;
    delete[] anruferT;
    sem_destroy(&incomingCall);

    return 0;
}
