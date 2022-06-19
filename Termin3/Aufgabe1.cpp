#include <iostream>
#include <queue>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <semaphore.h>
#include <chrono>
using namespace std;
using chrono::high_resolution_clock;
using chrono::seconds;
using chrono::duration_cast;
//Warteschlange
queue <int> Warteschlange;
//Counter
int AnruferID = 0;
int freeMitarbeiter;
//Anzahl der Mitarbeiter
int Anzahl;
//Anzahl der Anrufer
int Ruferanzahl;
//Semaphore
sem_t semFull;
sem_t semEmpty;
//Mutex
pthread_mutex_t M;
pthread_mutex_t M2;

//Funktion for Mitarbeiter
void* takeCall(void* args){
while (Ruferanzahl>0 || !Warteschlange.empty())
{
    sem_wait(&semFull);
    //Accept call
    pthread_mutex_lock(&M);
    std::cout<<"Accepting call from Anrufer ID "<< Warteschlange.front() <<endl;
    Warteschlange.pop();
    pthread_mutex_unlock(&M);
    //duration of the call is 2 seconds
    sleep (2);
    std::cout<<"End of conversation"<<endl;
    sem_post(&semEmpty);
}
return NULL;
}

//Funktion for Anrufer
void* makeCall(void* args){
while(true){
//Check warteschlange
//if not full go to the warteschlange
 pthread_mutex_lock(&M2);
    if(Warteschlange.size() < 15){     
        Warteschlange.push(++AnruferID);
        //Check Semaphore
        sem_wait(&semEmpty);      
        sem_post(&semFull);
        // std::cout << "Anrufer " << AnruferID << ": going out\n";
        pthread_mutex_unlock(&M2);
        break;
    }
//if full wait 5 seconds and make another call
    else{
        pthread_mutex_unlock(&M2);
        sleep (5);
    }
}
Ruferanzahl--;
// std::cout << "Ruferanzahl: " << Ruferanzahl << '\n';
return NULL;
}
int main(){

    //chrono
    high_resolution_clock::time_point my_end {};
    high_resolution_clock::time_point my_start{high_resolution_clock::now()};

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

    //Assign thread as Mitarbeiter
    pthread_t* mitArbeiter = new pthread_t[Anzahl];
    for(int i=0; i<Anzahl; i++){
        pthread_create(&mitArbeiter[i],NULL,takeCall,NULL);
    }

    //Assign thread as Anrufer
    const int rufer_anzahl = Ruferanzahl;
    pthread_t* Anrufer = new pthread_t[Ruferanzahl];
    for(int i=0; i<rufer_anzahl; i++){
        pthread_create(&Anrufer[i],NULL,makeCall,NULL);
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
    pthread_mutex_destroy(&M2);
    my_end = high_resolution_clock::now();
    seconds s{duration_cast<seconds>(my_end-my_start)};
    std::cout<<"durchschnittliche Wartezeit =" <<((s.count()-(2*Ruferanzahl))/Ruferanzahl)<<" Seconds"<<endl;
    std::cout << "Programm fertig\n";
    return 0;
}