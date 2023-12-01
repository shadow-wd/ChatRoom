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


#define RECVBUFFERSIZE 2048

void *rec_func(void *arg){
    char buffer[RECVBUFFERSIZE];
    int nbytes;
    int fd = *(int *)arg;
    LOGI("fd = %d\n",fd);
    free(arg);
    while(1){
        nbytes = recv(fd,buffer,RECVBUFFERSIZE,0);
        if(nbytes == -1){
            fprintf(stderr,"recv fail!\n");
            close(fd);
            break;
        }else if(nbytes == 0){
            close(fd);
            printf("client disconnect!\n");
            break;
        }
        buffer[nbytes] = '\0';
        LOGI("nbytes - %d\n",nbytes);
        LOGI("recv data = %s\n",buffer);

        if(send(fd,buffer,sizeof(buffer),0) == -1){
            fprintf(stderr,"server data to client fail!\n");
            exit(1);
        }
    }

}

int main(int argc,char *argv[]){
    int portnumber;
    int clientlen;
    int sockfd,clientfd;
    struct sockaddr_in server_addr,client_addr;
    int *pthreadarg = NULL;
    pthread_t thread;


    if(argc != 2){
        fprintf(stderr,"input invalid!\n");
        exit(1);
    }


    if( (portnumber = atoi(argv[1])) < 0){
        fprintf(stderr,"change portnumber faild\n");
        exit(1);
    }

    LOGI("portnumber = %d\n",portnumber);

    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        fprintf(stderr,"socket create failed!\n");
        exit(1);
    }    

    LOGI("socketfd = %d\n",sockfd);
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_port = htons(portnumber);
    LOGI("port : %u\n",server_addr.sin_port);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    LOGI("addr : %u\n",server_addr.sin_addr.s_addr);
    

    if( (bind(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))) == -1){
        fprintf(stderr,"socket bind fail!\n");
        exit(1);
    }
    
    LOGI("bind Successfully!\n");
    if( (listen(sockfd,10)) == -1){
        fprintf(stderr,"socket listen fail!\n");
        exit(1);
    }
    LOGI("start listen socket!\n");

    clientlen = sizeof(client_addr);
    while(1){
        if((clientfd = accept(sockfd,(struct sockaddr *)&client_addr,&clientlen)) == -1){
            fprintf(stderr,"accept socket fail!\n");
            exit(1);
        }

        pthreadarg = (int *)malloc(sizeof(int));
        *pthreadarg = clientfd;
        LOGI("clientfd = %d\n",clientfd);

        if(pthread_create(&thread,NULL,rec_func,(void *)pthreadarg) != 0 ){
            fprintf(stderr,"pthread create fail!\n");
            exit(1);
        }
        LOGI("create recv pthread Successfully!\n");

    }

    return 0;

}