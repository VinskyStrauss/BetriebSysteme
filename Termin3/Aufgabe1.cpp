#include <iostream>
#include <queue>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <semaphore.h>
using namespace std;
//Warteschlange
queue Warteschlange:
//Counter
int AnruferID = 0;
int* linie;
int freeMitarbeiter;
//Semaphore
sem_t semFull;
sem_t semEmpty;
//Mutex
pthread_mutex_t M;
pthread_mutex_t M2:

//Funktion for Mitarbeiter
void takeCall(){
while (true)
{
    sem_wait(&semFull);
    //Accept call
    M.lock();
    std::cout<<"Accepting call from Anrufer ID"<< Warteschlange.front() <<endl;
    Warteschlange.pop();
    M.unlock();
    //duration of the call is 2 seconds
    sleep (2);
    std::cout<<"End of conversation"<<endl;
    sem_post(&semEmpty);
}
}

//Funktion for Anrufer
void makeCall(){
while(true){
//Check warteschlange
//if not full go to the warteschlange
 M2.lock();
    if(Warteschlange.size() < 15;){     
        Warteschlange.push(++AnruferID);
        M2.unlock();
        //Check Semaphore
        sem_wait(&semEmpty);      
        sem_post(&semFull);
    }
//if full wait 5 seconds and make another call
    else{
        M2.unlock();
        sleep (5);
    }
  
}
}
int main(){
    //Anzahl der Mitarbeiter
    int Anzahl;
    //Anzahl der Anrufer
    int Ruferanzahl;

    cout<<"Anzahl Mitarbeiter: ";
    cin>>Anzahl;
    cout<<"Anzahl der Anrufer: ";
    cin>>Ruferanzahl;

    //freemitarbeiter
    freeMitarbeiter = Anzahl;

    //Semaphore initialisation
    sem_init(&semFull,0,0);
    sem_init(&semEmpty,0,Anzahl);
 
    //Mutex initialisation
    pthread_mutex_init(&M,NULL);
    pthread_mutex_init(&M2,NULL);


    //Assign thread as Anrufer
    pthread_t* Anrufer = new pthread_t[Ruferanzahl];
    for(int i=0; i<Ruferanzahl; i++){
        pthread_create(&Anrufer[i],NULL,makeCall,NULL);
    }

    //Assign thread as Mitarbeiter
    pthread_t* mitArbeiter = new pthread_t[mitArbeiter];
    for(int i=0; i<Anzahl; i++){
        pthread_create(&mitArbeiter[i],NULL,takeCall,NULL);
    }
    //Join Anrufer
    for(int i=0; i<Ruferanzahl; i++){
        pthread_join(Anrufer[i],NULL);
    }
    //Join Mitarbeiter
    for(int i=0; i<Anzahl; i++){
        pthread_join(mitArbeiter[i],NULL);
    }

    //Semaphore destroy
    sem_destroy(&semFull);
    sem_destroy(&semEmpty);

    //Mutex destroy
    pthread_mutex_destroy(&M);
    pthread_mutex_init(&M2);
}