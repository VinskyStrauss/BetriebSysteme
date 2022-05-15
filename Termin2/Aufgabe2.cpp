#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <signal.h>

void run(std::string command);
void logoutHandler(std::string command);
void sigTstpHandler(int signum);
void sigIntHandler(int signum);
void sigChldHandler(int signum);

pid_t pid;
std::vector<pid_t> liste;

//pid foreground
std::vector<pid_t> vordergrund;
//pid background
std::vector<pid_t> hintergrund;

int main(){
    signal(SIGTSTP, sigTstpHandler);
    signal(SIGINT, sigIntHandler);
    signal(SIGCHLD, sigChldHandler);

    while (1)
    {
        std::cout << "> ";
        std::cin.clear();
        std::string command; //buffer for command
        std::getline(std::cin, command);

        run(command);
    }

    return 0;
}

void sigTstpHandler(int signum){ //signal stop handler CTRL+Z
    std::cout << "[SIGTSTP: pid " << pid << "]\n";
}

void sigIntHandler(int signum){//Signal terminate handler CTRL+C
    std::cout << "[SIGINT: pid " << pid << "]\n";
}

void sigChldHandler(int signum){ //Signnal child handler
    int status;
    int mypid = waitpid(pid, &status, WNOHANG);
    
    if(mypid > 0 && WIFEXITED(status)){
        for(int i = 0; i < liste.size(); i++){
            if(liste.at(i) == mypid){
                std::cout << mypid << " was stopped\n";
                liste.erase(liste.begin() + i);
                break;
            }
        }
    }
}

void run(std::string command){
        bool pipeFlag = 0;
        
        if(!command.empty() && command != "logout"){

            if(command.at(command.size() - 1) == '&'){
                pipeFlag = 1;
                command.erase(command.begin() + command.size() - 1); //Loesche das &-Zeichen
            }

            int numOfCommands = 1; //NULL am Ende
            for(int i = 0; i < command.size(); i++){
                if(command.at(i) == ' ') numOfCommands++;
            }   

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
            
            if(command.at(0) == 's' && command.at(1) == 't' && command.at(2) == 'o' && command.at(3) == 'p' && command.at(4) == ' '){
                std::string s_stopPid = "";

                for(int i = 5; i < command.size(); i++){
                    s_stopPid += command.at(i);
                }

                pid = std::stoi(s_stopPid);
                kill(pid, SIGTSTP);
                

                return;
            }

            if(command.at(0) == 'f' && command.at(1) == 'g' && command.at(2) == ' ' && !vordergrund.empty()){ //Continue foreground Process
                std::string s_contPid = "";

                for(int i = 3; i < command.size(); i++){
                    s_contPid += command.at(i);
                }

                pid = std::stoi(s_contPid);
                for(unsigned int i=0; i<vordergrund.size(); i++){
                    if(pid == vordergrund.at(i)){
                     kill(pid, SIGCONT);
                     return;   
                    }
                }

              
            }

            if(command.at(0) == 'b' && command.at(1) == 'g' && command.at(2) == ' ' && !hintergrund.empty()){ //Continue foreground Process
                std::string s_contPid = "";

                for(int i = 3; i < command.size(); i++){
                    s_contPid += command.at(i);
                }

                pid = std::stoi(s_contPid);
                for(unsigned int i=0; i<hintergrund.size(); i++){
                    if(pid == hintergrund.at(i)){
                     kill(pid, SIGCONT);
                     return;   
                    }
                }
                
                
            }

            pid = fork();
            if(pid ==  -1){
                std::cout << "Failed forking\n";
                return;
            }

            else if(pid == 0){
                if(execvp(args[0], args) == -1){ 
                    std::cout << "Failed running command\n";
                    exit(-1);
                }
            }
            else{
                if(pipeFlag == 0){
                    vordergrund.push_back(pid);
                    waitpid(pid, NULL, WUNTRACED);
                }

                else if(pipeFlag == 1){   
                    hintergrund.push_back(pid);    
                    waitpid(pid, NULL, WNOHANG);             
                    for(int i = 0; i < liste.size(); i++){
                        std::cout << "Hintergrund: [" << liste.at(i) << "]\n";
                    }
                }
            }
        }

        else if(command == "logout"){
            
            if(!liste.empty()){
                std::cout << "Logout failed.\nRunning background process(es):\n";
                for(int i = 0; i < liste.size(); i++){
                    std::cout << i + 1 << ". " << liste.at(i) << "\n";
                }
                return;
            }

            else{
                logoutHandler(command);
            }
        }
            
}

void logoutHandler(std::string command){
    std::cout << "Wollen Sie die Shell wirklich beenden (J/N)? ";
    
    char c;
    std::cin >> c;

    if(c == 'J' || c == 'j'){
        exit(0);
    }

    else{
        return;
    }
    
}