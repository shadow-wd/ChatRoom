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
#include "../database/opera_db.h"
#include "opera_server.h"

void *rec_func(void *arg){
    int len;
    struct protocol msg;
    int fd = *(int *)arg;
    LOGI("fd = %d\n",fd);
    free(arg);
    while(1){
        // read cmd
        len = read(fd,&msg,sizeof(msg));
        LOGI("len = %d\n",len);
        if(len == 0){
            // recv fail
            fprintf(stderr,"client disconnect!\n");

            opera_logout(fd);
            close(fd);
            break;
        }else if(len < 0){
            // recv fail
            fprintf(stderr,"read data fail!\n");
            opera_offline(msg.name);
            close(fd);
            break;
        }
        // printf("msg->name%s\n",msg.name);
        switch (msg.cmd)
        {
        case LOGIN:
            opera_login(fd,&msg);
            break;
        case BROADCAST:
            break;
        case PRIVATE:
            break;
        case REGISTER:
            opera_register(fd,&msg);
            break;
        case ONLINEUSER:
            opera_show_online(fd);
            break;
        case LOGOUT:
            opera_logout(fd);
            break;
        default:
            break;
        }
        // printf("msg->111name%s\n",msg.name);
    }
    printf("exit the recv pthread!\n");
    pthread_exit(NULL);

}

int main(int argc,char *argv[]){
    int clientlen;
    int sockfd,clientfd;
    struct sockaddr_in server_addr,client_addr;
    int *pthreadarg = NULL;
    pthread_t thread;


    int choice;
    char name[50];
    int fd;
    char passwd[50];
    int flag;

    if(db_init(&db) == -1){
        printf("database fail!\n");
        return;
    }


    if(argc != 1){
        fprintf(stderr,"input invalid!\n");
        exit(1);
    }


    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        fprintf(stderr,"socket create failed!\n");
        exit(1);
    }  

    LOGI("socketfd = %d\n",sockfd);
    bzero(&server_addr,sizeof(struct sockaddr_in));

    // server_addr.sin_port = htons(portnumber);
    server_addr.sin_port = htons(SERVERPORT);
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