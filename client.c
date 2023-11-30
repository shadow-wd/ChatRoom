#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "log.h"


#define BUFFERSIZE 1024

void *rec_func(void *arg){
    char buffer[BUFFERSIZE];
    int nbytes;
    int fd = *(int *)arg;
    free(arg);
    while(1){
        nbytes = recv(fd,buffer,BUFFERSIZE,0);
        if(nbytes == -1){
            close(fd);
            break;
        }else if(nbytes == 0){
            close(fd);
            printf("server disconnect!\n");
            break;
        }
        buffer[nbytes] = '\0';
        printf("recv server data = %s\n",buffer);
    }

}

int main(int argc,char *argv[]){
    int portnumber;
    int sockfd;
    struct sockaddr_in server_addr;
    int *pthreadarg = NULL;
    pthread_t thread;
    char buffer[BUFFERSIZE];

    if(argc != 3){
        fprintf(stderr,"input invalid!\n");
        exit(1);
    }


    if( (portnumber = atoi(argv[2])) < 0){
        fprintf(stderr,"change portnumber faild\n");
        exit(1);
    }

    LOGI("portnumber = %d\n",portnumber);

    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        fprintf(stderr,"socket create failed!\n");
        exit(1);
    }    


    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_port = htons(portnumber);
    LOGI("port : %u\n",server_addr.sin_port);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    LOGI("addr : %u\n",server_addr.sin_addr.s_addr);
    
    if( (connect(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))) == -1){
        fprintf(stderr,"socket connect fail!\n");
        exit(1);
    }
    LOGI("client connect server!\n");
    pthreadarg = (int *)malloc(sizeof(int));
    *pthreadarg = sockfd;


    if(pthread_create(&thread,NULL,rec_func,(void *)pthreadarg) != 0 ){
        fprintf(stderr,"pthread create fail!\n");
        exit(1);
    }
    while(1){
        printf("input send mesggage:\n");
        scanf("%s",buffer);
        LOGI("send data = %s\n",buffer);
        if((send(sockfd,buffer,strlen(buffer),0)) == -1){
            fprintf(stderr,"send data to server fail!\n");
            exit(1);
        }

    }
    pthread_join(thread, NULL);
    return 0;

}