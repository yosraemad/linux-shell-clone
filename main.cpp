#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;

void logToFile(int signal) {
    ofstream file;
    file.open("log.log", ios_base::app);

    file << "Child process was terminated" << endl;
    file.close();
}

int main() {
    char command[1024];
    char * args[100];
    while(true) {
        printf("Shell> ");

        // --- Get Input from User ---
        fgets(command, 1024, stdin);

        // --- If the user typed exit ---
        if(!strcmp(command, "exit\n")) {
            break;
        }

        // --- convert the command to an array ---
        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        
        // --- remove \n at the end of the line ---
        args[i - 1][strlen(args[i - 1]) - 1] = '\0';

        // --- add null to the end of the array ---
        args[i] = NULL;

        // --- listen to SIGCHLD signal ---
        signal(SIGCHLD, logToFile);

        pid_t pid = fork();
        if(pid < 0) {
            // --- error in forking ---
            cout << "An error occurred" << endl;
        } else if (pid == 0) {
            // --- child process ---
            if(!strcmp(args[i - 1], "&")) {
                args[i - 1] = NULL;
            }
            if(execvp(args[0], args) == -1) {
                cout << "Invalid Command " << args[0] << endl;
            }
            return 900;
        } else {
            // --- parent process ---
            if(strcmp(args[i - 1], "&"))
                wait(NULL);
        }
    }
}