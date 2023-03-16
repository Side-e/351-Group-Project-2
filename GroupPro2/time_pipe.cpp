#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

using namespace std;

int main(int argc, char*argv[])
{
    struct timeval end_time, start_time, elapsed_time;

    string please;
    char test[50];
    int i = 0;
    cin>>please;
    fgets(test,50,stdin);//reads input from user

    char* temptest= test;
    char* thisone[sizeof(test)+1];
    char* l;

    //p [0] read
    //p [1] write
    int p[2];

    if(pipe(p) == -1)
    {
        cout << "Error occurred when opening pipe\n";
    }

    if(please.find("./time")!= std::string::npos)
    {
        l = strtok(temptest, " ");
        while(l != NULL)
        {
        thisone[i] = l;
        l = strtok(NULL, " ");
        i++;
        }
        thisone[i-1][strcspn(thisone[i-1], "\n")] = '\0';
        thisone[i] = NULL;
        int id = fork();

        //child process
        if (id == 0)
        {   
            close(p[0]);

            gettimeofday(&start_time, NULL);
            write(p[1], &start_time.tv_sec, sizeof(int));
            write(p[1], &start_time.tv_usec, sizeof(int));

            close(p[1]);
            
            execvp(thisone[0],thisone);
        }
        else //Parent
        {
            wait(NULL);
            close(p[1]);

            gettimeofday(&end_time, 0);
            read(p[0], &start_time.tv_sec, sizeof(int));
            read(p[0], &start_time.tv_usec, sizeof(int));

            close(p[0]);

            timersub(&end_time, &start_time, &elapsed_time);

            printf("Elapsed time: %ld.%06ld seconds\n",  elapsed_time.tv_sec, elapsed_time.tv_usec);
        }
    }
    return 0;
}