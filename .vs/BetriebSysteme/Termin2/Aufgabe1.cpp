#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>

void run();
bool logoutHandler(std::string command);

int main(){

    run();

    return 0;
}

void run(){

    std::vector<pid_t> liste; // liste of process id

    while (1) // unendliche Schleife
    {
        bool pipeFlag = 0;

        std::cout << "> ";
        std::string command; // buffer für Kommand
        std::getline(std::cin, command);
        
        if(!command.empty() && command != "logout"){ // Richtige Befehle wurde akzeptiert

            if(command.at(command.size() - 1) == '&'){ // command1 & // Hinttergrund Prozess
                pipeFlag = 1; // set true 
                liste.push_back(getpid()); // insert current process id to liste of process id 
                command.erase(command.begin() + command.size() - 1); //Loesche das &-Zeichen 
            }

            int numOfCommands = 1; //NULL am Ende
            for(int i = 0; i < command.size(); i++){
                if(command.at(i) == ' ') 
                    numOfCommands++;
            }
            // sudo apt-get dpkg proble.deb = 3 Befehle

            char* args[numOfCommands+1];
            std::string splitCommand[numOfCommands+1];

            for(int i = 0, j = 0; i < command.size() && j < numOfCommands; i++){
                if(command.at(i) == ' '){
                    j++;
                    i++;
                }
            splitCommand[j] += command.at(i);
            args[j] = (char*)splitCommand[j].c_str();

            }
            args[numOfCommands] = NULL;


            pid_t pid = fork();
            pid_t childPid;
            if(pid ==  -1){
                std::cout << "Failed forking\n";
                return;
            }

            else if(pid == 0){
                childPid = getpid();
                if(execvp(args[0], args) == -1){
                    std::cout << "Failed running command\n";
                    exit(-1);
                }
            }
            else{
                int status;
                if(pipeFlag == 0){
                    waitpid(pid, &status, WUNTRACED);
                }

                else if(pipeFlag == 1){
                    waitpid(pid, &status, WNOHANG);
                    std::cout << "Hintergrund: [" << liste.at(liste.size() - 1) << "]\n";
                }
            }
        }

        else if(command == "logout"){
            if(logoutHandler(command)){
                return;
            }
        }
        
        sleep(1); // damit sich die Eingabe des nächsten Befehls mit der Ausgabe des Prozesses
		 // nicht verschachtelt
    }
}

bool logoutHandler(std::string command){
    std::cout << "Wollen Sie die Shell wirklich beenden (J/N)? ";
    
    char c;
    std::cin >> c;

    if(c == 'J' || c == 'j'){
        return 1;
    }

    else{
        std::cin.clear();  
        std::cin.ignore();
        return 0;
    }
    
}