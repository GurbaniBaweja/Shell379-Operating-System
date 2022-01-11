#include <iostream>
#include <sys/types>
#include <sys/wait>
#include <fcntl>
#include <stdlib>
#include <string>
#include <unistd>
#include <stdio>
#include <sys/resource>
#include <sys/time>
#include <errorno>
#include <fstream>
#include <signal>

#define LINE_LENGTH 100
#define MAX_ARGS 7
#define MAX_LENGTH 20
#define MAX_PT_ENTRIES 32
bool given_file = false;
string out_file = "";
using namespace std;

//Not named exit alone unlike others because of reserved keyword
void exitShell(){
    // Implemented using these two resources: 
    // 1. https://stackoverflow.com/questions/10509660/getting-getrusage-to-measure-system-time-in-c
    // 2. https://pubs.opengroup.org/onlinepubs/009696699/functions/getrusage.html
    int user_time_milli, sys_time_milli, user_time_sec, sys_time_sec = 0;
    struct rusage user_usage, sys_usage;
    getrusage(RUSAGE_SELF, &user_usage);
    getrusage(RUSAGE_CHILDREN, &sys_usage);
    user_time_milli = user_usage.ru_stime;
    user_time_sec = user_time_milli.tv_sec;  //converting milliseconds to seconds
    sys_time_milli = sys_usage.ru_stime;
    sys_time_sec =  sys_time_milli.tv_sec;   //converting milliseconds to seconds
    
    // Writing to file if an output file is given
    if (given_file){
        ofstream f;
        f.open(out_file);
        f<<"Resources Used" << endl;
        f<<"User time =     "<<user_time_sec<<" seconds" << endl;
        f<<"Sys time =     "<<sys_time_sec<<" seconds" << endl;
        f.close();
    }
    else{
        cout<< "\n";
        cout<<"\n Resources Used";
        cout<<"\n User time =     "<<user_time_sec<<" seconds";
        cout<<"\n Sys time =     "<<sys_time_sec<<" seconds";
    }
    
    exit(0);
}

void jobs(){
    //Resources : Lab 3 and https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po  
    char buf[MAX_LENGTH];
    int processes_num;
    FILE* fp;
    fp = popen ("ls -l","r");
    while (fgets(buf, MAX_LENGTH, fp) != NULL){
        processes_num = processes_num + buf;
    }
    pclose(fp);

    int user_time_milli, sys_time_milli, user_time_sec, sys_time_sec = 0;
    struct rusage user_usage, sys_usage;
    getrusage(RUSAGE_SELF, &user_usage);
    getrusage(RUSAGE_CHILDREN, &sys_usage);
    user_time_milli = user_usage.ru_stime;
    user_time_sec = user_time_milli.tv_sec;  //converting milliseconds to seconds
    sys_time_milli = sys_usage.ru_stime;
    sys_time_sec =  sys_time_milli.tv_sec;   //converting milliseconds to seconds
    
    // Writing to file if an output file is given
    if (given_file){
        ofstream f;
        f.open(out_file);
        f<<"Running processes:" << endl;
        f<<"Processes = "<< processes_num << " active" <<endl;
        f<<"Completed Processes:" << endl;
        f<<"User time =     "<<user_time_sec<<" seconds" << endl;
        f<<"Sys time =     "<<sys_time_sec<<" seconds" << endl;
        f.close();
    }
    else{
        cout<< "\n";
        cout<<"\n Running Processes:";
        cout<<"\n Processes = "<< processes_num <<" active";
        cout<<"\n Completed Processes";
        cout<<"\n User time =     "<<user_time_sec<<" seconds";
        cout<<"\n Sys time =     "<<sys_time_sec<<" seconds";
    }
}

void kill(){
    //Reference: Lab 3
    int pid = fork();
    if (pid>0){
        sleep(1);
        kill(pid,SIGKILL);
    }
}

void resume(){
    //SIGCONT discovered from : https://major.io/2009/06/15/two-great-signals-sigstop-and-sigcont/#:~:text=the%20SIGCONT%20signal.-,SIGSTOP%20and%20SIGCONT%20are%20used%20for%20job%20control%20in%20the,up%20where%20you%20left%20off%E2%80%9D.
    int pid = fork();
    if (pid>0){
        sleep(1);
        kill(pid,SIGCONT);
    }
}

void sleep(){
    int pid = fork();
    if (pid>0){
        sleep(1);
    }
}

void suspend(){
    //SIGSTOP discovered from : https://major.io/2009/06/15/two-great-signals-sigstop-and-sigcont/#:~:text=the%20SIGCONT%20signal.-,SIGSTOP%20and%20SIGCONT%20are%20used%20for%20job%20control%20in%20the,up%20where%20you%20left%20off%E2%80%9D.
    int pid = fork();
    if (pid>0){
        sleep(1);
        kill(pid,SIGSTOP);
    }
}


//Not named wait alone unlike others because of reserved keyword
void waitShell(){
    //Reference: Lab 2 
    int pid = fork();
    if (pid>0){
        exit(0);
    }
    else{
        wait(NULL);
    }   
}

void background_process(){
    //Code adapted from Chapter 5 Three Easy Pieces and Lab2 
    int rc = fork();
    int pid =0;
    if (rc<0){
        cout<<"\n Fork failed";
        exit(1);
    }
    else if (rc == 0){
        pid = getpid();
    }
    else{
        wait (NULL);
        pid = getpid;
    }

}

void read_file(string filename){
    ofstream f;
    f.open(filename);
    while ( getline(f,line,'')){
        for (int i = 0; i<7; i++){
            if (line[i].compare("exit")==0){
                exitShell();
            }
            else if (line[i].compare("jobs")==0){
                jobs();
            }
            else if (line[i].compare("kill")==0){
                kill();
            }
            else if (line[i].compare("resume")==0){
                resume();
            }
            else if (line[i].compare("sleep")==0){
                sleep();
            }
            else if (line[i].compare("suspend")==0){
                suspend();
            }
            else if (line[i].compare("wait")==0){
                waitShell();
            }
            else(){}

            if (line[i].compare("&")==0){
                background_process();
            }

        }
    }
    f.close();
}


int main(int argc, char *argv[]){ 
    if (argc <= MAX_ARGS){
        for (int i = 0; i<argc; i++){
            if (argv[i].compare("exit")==0){
                exitShell();
            }
            else if (argv[i].compare("jobs")==0){
                jobs();
            }
            else if (argv[i].compare("kill")==0){
                kill();
            }
            else if (argv[i].compare("resume")==0){
                resume();
            }
            else if (argv[i].compare("sleep")==0){
                sleep();
            }
            else if (argv[i].compare("suspend")==0){
                suspend();
            }
            else if (argv[i].compare("wait")==0){
                waitShell();
            }
            else(){}

            if (argv[i].compare("&")==0){
                background_process();
            }

            if (argv[i].compare("<")==0){
                string filename = argv[i+1];
                read_file(filename);
            }

            if (argv[i].compare(">")==0){
                string out_file = argv[i+1];
                given_file = true;
            }
        }
    }


}
