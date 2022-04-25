#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//extra lib
//#include <errno.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
// This is a bare-bone TEDShell
// You need to write your code to work as described in the document.

// Use following command to compile:
// g++ TEDShell.c -o TEDShell

// To run the program
// ./TEDShell

// To run testcases
// ./TEDShell-tester
#define MAX_PARAMS 50 //max number of parameter
#define MAX_CMD_LENGTH 1024 // max command length 1024 characters

int a = 0;
char *params[MAX_PARAMS+1];

// Builtin function implementations.
int shell_cd()
{
  if (params[1] == NULL) {
    fprintf(stderr, "Missing argument. Expected argument after \"cd\"\n");

  } else if((params[1] != NULL)) {
        if (chdir(params[1]) != 0) {
            perror("shell_cd");

    }
  }
  return 1;
}

//executes the command in a threaded fashion for interactive mode
void *execute() {
    pid_t pid;
    pid = fork();

    if (pid == 0) {
        //command: ls
        if (execvp(params[0], params) < 0) {

        }
    } else if (pid < 0) { 
        perror("errorrrrrrrrrr");
    }

    // pthread_exit(NULL);
}

//for quitting the interactive mode in a threaded fashion
void *quit(void *threadID) {
	long tid=(long ) threadID;
	sleep(1);
	exit(0);
	pthread_exit(NULL);
}

void readline(char* buffer){
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

//split the command into an array of params
int parsecmd(char* input, char** params) {
    int n =0;
    for(int i = 0; i < MAX_PARAMS; i++) {
        params[i] = strsep(&input, " "); // ";" sildik 

        n++;
        if(params[i] == NULL) {
          //  params[i] = NULL;
            break;
        }
    }
    // return (sizeof(params)/sizeof(int)) ; //returns the length
    return (n - 1) ; //returns the length

}


char *builtin_isItCMD[] = {
    "cd",
    "path",
    "exit"
};

int main(int argc, char *argv[])
{
    char cmd[MAX_CMD_LENGTH + 1];
    FILE *fptr;
    char file[100];
    pthread_t threads[32];
    char input[100];
    char builtInCmd[100] = "false";

    //taking input
    printf("TEDU_Shell 🚀 > ");
    readline(input);

    do {

        int j = parsecmd(input, params); //parsing of cmd into params array
       // printf("%d\n", j);

        for (int i = 0; i < sizeof(builtin_isItCMD) / sizeof(char *); i++) { //searching in builtin_isItCMD 
            if (strcmp(params[0], builtin_isItCMD[i]) == 0) {
                strcpy(builtInCmd, builtin_isItCMD[i]);
            }
        }

    	for(int i = 0; i < j; i++) { 
            
            //for built in
            if (strcmp(builtInCmd, "false") != 0) {

                if((strcmp(params[0], "exit") == 0)) {

                    if(params[1] == NULL) {
                        printf("Bye bye...\n");

                        int ls = pthread_create(&threads[i], NULL, quit, (void *)(intptr_t)i);

                    } else {
                        printf("An error has occurred\n");

                    }
                    strcpy(builtInCmd, "false"); // checking for built or not

                } else if (strcmp(params[0], "cd") == 0){
                        int ls = pthread_create(&threads[i], NULL, shell_cd, (void *)(intptr_t)i); 
                        strcpy(builtInCmd, "false");

                } else if (strcmp(params[0], "path") == 0) {
                       
                    strcpy(builtInCmd, "false");
                }

            } else if (strcmp(builtInCmd, "false") == 0) {

               // for (int m = 0; m < j; m++) {  // ">" method
                    //printf("params2: %s bu da j: %d buda m: %d \n",params[m], j, m);
                    if (strcmp(params[i], ">") == 0) {
                        strcpy(file, params[i + 1]);
                        fptr = fopen(file, "w");
                        
                        if (fptr == NULL) {
                             printf("Error!");   
                             exit(1);             
                        }
                        //fprintf(fptr,"%d",num);
                        //fclose(fptr);
                        //fgets(file, sizeof(file), stdin);

                        //fputs(execute, fptr);
                        //printf("fgets mi \n");
                        //fprintf(fptr, "%s", file);

                        fwrite(stdout, 1000, sizeof(file), fptr);
                        // printf("%s\n", file);
                        fclose(fptr);
                         
                    }
              //  }

                if (strcmp(params[0], "ls") == 0) {
                    int ls = pthread_create(&threads[i], NULL, execute, (void *)(intptr_t)i);
                    strcpy(builtInCmd, "false");

                } else {
                    int ls = pthread_create(&threads[i], NULL, execute, (void *)(intptr_t)i);
                    strcpy(builtInCmd, "false");


                }
  
            }
            printf("You provided the command: %s\n", input);

        }

        sleep(1);
      
        printf("TEDShell 🍆 > ");
        readline(input);    
    } while (strcmp(input, "exit") != 0);

    


    return 0;
} 