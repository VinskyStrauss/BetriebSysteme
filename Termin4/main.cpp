//Termin4
#include <iostream>
#include <queue>
#include <stack>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <semaphore.h>
using namespace std;
//Warteschlange
std::stack <int> Warteschlange;
//Counter
int AnruferID = 0;
int freeMitarbeiter;
//Anzahl der Mitarbeiter
int Anzahl;
//Anzahl der Anrufer
int Ruferanzahl;
//Anrufer counter
int AnrufCounter;
//Semaphore für Mitarbeiter
sem_t semFull;
sem_t semEmpty;

sem_t semMitarbeiter;
//Semaphore für Warteschlange
sem_t semFullWarte;
sem_t semEmptyWarte;
//Mutex
pthread_mutex_t M;
pthread_mutex_t M2;

//Funktion for Mitarbeiter
void* takeCall(void* args){
    sleep(5);
    struct timespec ts; 
    ts.tv_sec = 20;
    while (!sem_timedwait(&semFullWarte, &ts))
    {
        //Accept call
        sem_wait(&semMitarbeiter);
        pthread_mutex_lock(&M);
        int id = Warteschlange.top();
        AnrufCounter--;
        std::cout<<"Accepting call from Anrufer ID "<< id <<endl;
        Warteschlange.pop();
        pthread_mutex_unlock(&M);
        sem_post(&semEmptyWarte);
        //duration of the call is 10 seconds
        sleep (5);
        sem_post(&semMitarbeiter);
        std::cout<<"End of conversation " << id <<endl;
    }
    std::cout<<"Mitarbeiter exiting" <<endl;
    return NULL;
}

//Funktion for Anrufer
void* makeCall(void* args){
    while(AnruferID < 15){
    //Check warteschlange
    //if not full go to the warteschlange
        if(Warteschlange.size() < 15){    
            sem_wait(&semEmptyWarte);
            pthread_mutex_lock(&M2);
            //Check Semaphore
            Warteschlange.push(++AnruferID);
            std::cout << "Added Anrufer " << AnruferID << std::endl;
            sem_post(&semFullWarte);
            // std::cout << "Anrufer " << AnruferID << ": going out\n";
            pthread_mutex_unlock(&M2);
            break;
        }
    // if full wait 5 seconds and make another call
        else{
            // pthread_mutex_unlock(&M2);
            sleep (5);
        }
    }
    Ruferanzahl--;
    // std::cout << "Ruferanzahl: " << Ruferanzahl << '\n';
    return NULL;
}
int main(){

    cout<<"Anzahl Mitarbeiter: ";
    cin>>Anzahl;
    cout<<"Anzahl der Anrufer: ";
    cin>>Ruferanzahl;

    //freemitarbeiter
    freeMitarbeiter = Anzahl;

    //Semaphore initialisation
    sem_init(&semMitarbeiter,0,Anzahl);

    sem_init(&semFullWarte,0,0);
    sem_init(&semEmptyWarte,0,15);
 
    //Mutex initialisation
    pthread_mutex_init(&M,NULL);
    pthread_mutex_init(&M2,NULL);


    //Assign thread as Anrufer
    const int rufer_anzahl = Ruferanzahl;
    pthread_t* Anrufer = new pthread_t[Ruferanzahl];
    for(int i=0; i<rufer_anzahl; i++){
        pthread_create(&Anrufer[i],NULL,makeCall,NULL);
    }

    //Assign thread as Mitarbeiter
    pthread_t* mitArbeiter = new pthread_t[Anzahl];
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
    sem_destroy(&semEmptyWarte);
    sem_destroy(&semFullWarte);
    sem_destroy(&semMitarbeiter);

    //Mutex destroy
    pthread_mutex_destroy(&M);
    pthread_mutex_destroy(&M2);
    std::cout << "Programm fertig\n";
    return 0;
}