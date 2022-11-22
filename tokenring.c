#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#ifndef __cplusplus
#include <stdbool.h>
#endif

char *int_to_str(int num, char *str)
{
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}

bool lock(float probability) {
  return rand()%100 < (probability * 100);
}

void test_debug(){
/*
    printf("%d , %f , %d ", n,p,time);
    printf("\n");

    for (int i = 0; i < 10000; i++){
        printf("%d", yesOrNo(p));
    }


*/
}
char* fillPipesPath(char *fifos, int n){
    
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
    float p =  (atof(argv[2]) >= 0.001 && atof(argv[2]) <= 1.0) ? atof(argv[2]) : 0.001;
    // Time -- time > 0;
    int time = atoi(argv[3]) > 0 ? atoi(argv[3]) : 5;
	// File descriptors for pipes i and i+1 and token
	int fd1, fd2, token = 0;
    // Pipes path array
    char* fifos[n][30];


    //      Generate path of pipes
    char filepath[80];
    int j = 0;
    char str[5];
    for(int i = 1; i <= n ; i++){
        strcpy(filepath,"./temp/pipe");
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

    
    for (int i = 0; i < n; i++){
        printf("%s", fifos[i]);
        printf("\n");}
    fd1 = open(fifos[0], O_WRONLY);
    close(fd1);

        

}


