#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif
#define MAX 80
#define SIZE sizeof(int);

// Helper functions
char *int_to_str(int num, char *str)
{
    if(str == NULL)
    {
        return NULL;
    }
    sprintf(str, "%d", num);
    return str;
}

char *slice(char *str, int start, int end)
{

    int i;
    int size = (end - start) + 1;
    char *output = (char *)malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++)
    {
        output[i] = str[start];
    }

    output[size] = '\0';

    return output;
}
void print_error(bool r, char* filepath){
    if(r){
        printf("Failed to open file%s\n",filepath);
    }else{
        printf("Failed to read/write to file%s\n",filepath);
    }
}

// bloqueio de envio da token
bool lock(float probability) {
    return rand()%100 < (probability * 100);
}


int main(int argc, char** argv) {
    if(argc < 4){
        printf("Usage: ./tokenring n_proc prob time\n\r");
        return EXIT_FAILURE;
    }

    //    Input validation, definitions

    // Nr of processes -- n > 1 (not sure)
    int n = atoi(argv[1]) > 0 ? atoi(argv[1]) : 2;
    // Probabilty -- 100% >= p >= 1%
    float probability =  (atof(argv[2]) >= 0.001 && atof(argv[2]) <= 1.0) ? atof(argv[2]) : 0.001;
    // Time -- time > 0;
    int time = atoi(argv[3]) > 0 ? atoi(argv[3]) : 5;
    // File descriptors for pipes i and i+1 and token
    int token = 0;
    char* dest;
    pid_t   pid[n];
    // Pipes path array
    char* fifos[n][MAX];
    int tokenread=0;


    //          Generate path of pipes
    char filepath[MAX];
    int j = 0;
    char str[5];
    for(int i = 1; i <= n ; i++){
        strcpy(filepath,"./tmp/pipe");
        strcpy(str,"");
        strcat(filepath,int_to_str(i,str));
        strcat(filepath,"to");
        if(i == n){
            strcat(filepath,"1");
        }else{
            strcat(filepath,int_to_str(i+1,str));
        }
        strcpy(fifos[j],filepath);
        j++;
    }
    //

    //   Create pipes (Temporary files in ./tmp)
    for(int i =0 ; i < n ; i++){
        char* fileToCreate = fifos[i];
        if ((mkfifo(fileToCreate,0666)) != 0) {
            if(errno == 17){ // If a file with the same name exists, this overwrites it
                unlink(fileToCreate);
                mkfifo(fileToCreate,S_IRWXU);
            }else{
                printf("Unable to create a fifo; errno=%d\n",errno);
                exit(1);
            }
        }
    }
    printf("hello\n");



    int itr = 0;
    file1 = fifos[itr];

    fd1 = open(file1,O_RDWR);

    if(fd1 == -1){
        print_error(1,file1);
        return 2;
    }
    if(write(fd1,&token,sizeof(token)) == -1){
        print_error(0,file1);
        return 2;
    }
    close(fd1);

    char *file1=(char*)malloc(50*sizeof(char));
    char *file2=(char*) malloc(50*sizeof(char));
    for (int i=1; i <= n; i++) {
        if((pid[i-1] = fork()) < 0) {
            fprintf(stderr, "./tokenring: fork error: %s\n", strerror(errno));
            return 2;
        } else if(pid[i-1] == 0) {
            if(i == n) {
                sprintf(file1, "pipe%dto1", i);
                sprintf(file2, "pipe%dto%d", i-1, i);
            } else if(i == 1) {
                sprintf(file1, "pipe%dto%d", i, i+1);
                sprintf(file2, "pipe%dto1", n);
            } else {
                sprintf(file1, "pipe%dto%d", i, i + 1);
                sprintf(file2, "pipe%dto%d", i - 1, i);
            }
            int fd[2];
            if(i==1) {
                if ((fd[1] = open(file1, O_WRONLY)) < 0) {
                    fprintf(stderr, "./tokenring: pipe opening error: %s\n", strerror(errno));
                    return 2;
                }

                val++; //todo

                if (write(fd[1], &tokenread, sizeof(int)) < 0) {
                    fprintf(stderr, "./tokenring: write error: %s\n", strerror(errno));
                    return 2;
                }

                close(fd[1]);
            }
            while (true) {
                if ((fd[0] = open(file2, O_RDONLY)) < 0) {
                    fprintf(stderr, "./tokering: pipe opening error: %s\n", strerror(errno));
                    return 2;
                }
                if (read(fd[0], &tokenread, sizeof(int)) < 0) {
                    fprintf(stderr, "./tokenring: read error: %s\n", strerror(errno));
                    return 2;
                }
                close(fd[0]);

                tokenread++;

                if (lock(probability)) {
                    printf("[p%s] lock on token (val = %d)\n", slice(file1, 10, 10), tokenread);
                    sleep(time);
                    printf("[p%s] unlock on token\n", slice(file2, 10, 10));
                }
                if((fd[1] = open(file1, O_WRONLY)) < 0) {
                    fprintf(stderr, "./tokenring: pipe opening error: %s\n", strerror(errno));
                    return 2;
                }

                if(write(fd[1], &tokenread, sizeof(int)) < 0) {
                    fprintf(stderr, "./tokenring: write error: %s\n",strerror(errno));
                    return 2;
                }
                close(fd[1]);
            }
            return 0;
        }
        for(int i =0 ; i < n ; i++){
            if(waitpid(pid[i], NULL, 0) < 0) {
                fprintf(stderr, "./tokenring: waitpid error: %s\n",strerror(errno));
                return 2;
            }
        }



    }
}

    void test_debug(){
/*
    printf("%d , %f , %d ", n,p,time);
    printf("\n");
    for (int i = 0; i < 10000; i++){
        printf("%d", yesOrNo(p));
    }
    for (int i = 0; i < n; i++){
        printf("%s", fifos[i]);
        printf("\n");}
        for (int i = 0; i < n; i++){
        printf("%s\n", fifos[i]);
        char* filee = fifos[i];
        printf("[p%s]lock on token val(%d)\n",slice(fifos[i],10,10),token);
        }
*/
    }
