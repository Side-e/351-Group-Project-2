#include <iostream>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>


#define SHM_SIZE 1024

using namespace std;

int main()
{
    int shmid;
    char *shm, *s;
    pid_t pid;
    struct timeval start_time, end_time, elapsed_time;
    string please;
    cout << "Hello world!" << endl;
    char test[50];
    int i = 0;
    cin>>please;
    fgets(test,50,stdin);//reads input from user

    //gettimeofday(&s,0);
    //cout<<s.tv_sec<<endl;
    char* temptest= test;
    char* thisone[sizeof(test)+1];
    char* l;
    // Create the shared memory segment
    if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment to our data space
    if ((shm = (char *) shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    //string please = temptest;// creates a string of the user input before strtok alters it
    if(please.find("./time")!= std::string::npos)
    {
        //char* arg0 = strtok(temptest, " ");
        //char* arg1 = strtok(NULL," ");
        //char *command = arg0;
        //thisone[0] = arg0;
        //thisone[1] = arg1;
        //thisone[2] = NULL;
        l = strtok(temptest, " ");
        while(l != NULL)
        {
        thisone[i] = l;
        l = strtok(NULL, " ");
        i++;
        }
        thisone[i-1][strcspn(thisone[i-1], "\n")] = '\0';
        thisone[i] = NULL;

        pid = fork();
        if(pid < 0)
        {
            exit(-1);// this shouldn't happen
        }
        else if(pid == 0)//child process
        {
        //system(arg1);
        // Get the starting time
        gettimeofday(&start_time, NULL);

        // Write the starting time to the shared memory segment
        sprintf(shm, "%ld %ld", start_time.tv_sec, start_time.tv_usec);

        execvp(thisone[0],thisone);


        }
        else//parent function
        {
        wait(NULL);
        cout<<"This is the parent function"<<endl<<endl<<endl;

        //execvp(thisone[0],thisone);

        // Read the starting time from the shared memory segment
        s = shm;
        char *sec = strsep(&s, " ");
        char *usec = strsep(&s, " ");
        start_time.tv_sec = atol(sec);
        start_time.tv_usec = atol(usec);

        // Get the ending time
        gettimeofday(&end_time, NULL);

        // Calculate the elapsed time
        timersub(&end_time, &start_time, &elapsed_time);

        // Print the elapsed time
        printf("Elapsed time: %ld.%06ld seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
        //timersub(&e,&s,&el);
       // cout<<"this took: "<<e.tv_sec<<" - "<<s.tv_sec<<" = "<<el.tv_sec<<" seconds"<<endl;

        }

        //cout<<"COMMAND TYPED BY USER:"<<endl<<please<<endl<<endl<<"ARGUEMENT 0: "<<arg1<<endl;
    return 0;
    }
}
