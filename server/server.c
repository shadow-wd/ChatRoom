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


#include "opera_server.h"



void *rec_func(void *arg){
    int len;
    struct protocol msg;

    int index = -1;
    int fd = *(int *)arg;
    LOGI("fd = %d\n",fd);
    free(arg);
    while(1){
        // read cmd
        len = read(fd,&msg,sizeof(msg));
        printf("%s:%d:index = %d\n",__func__,__LINE__,index);
        LOGI("len = %d\n",len);
        if(len == 0){
            // recv fail
            fprintf(stderr,"client disconnect!\n");
            printf("%s:%d:index = %d\n",__func__,__LINE__,index);
            opera_offline(index);
            close(fd);
            break;
        }else if(len < 0){
            // recv fail
            fprintf(stderr,"read data fail!\n");
            opera_offline(index);
            close(fd);
            break;
        }
        switch (msg.cmd)
        {
        case LOGIN:
            opera_login(fd,&index,&msg);
            break;
        case BROADCAST:
            opera_login(fd,&index,&msg);
            break;
        case PRIVATE:
            opera_login(fd,&index,&msg);
            break;
        case REGISTER:
            opera_register(fd,&index,&msg);
            break;
        case ONLINEUSER:
            opera_login(fd,&index,&msg);
            break;
        case LOGOUT:
            opera_login(fd,&index,&msg);
            break;
        default:
            break;
        }
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


    if(argc != 1){
        fprintf(stderr,"input invalid!\n");
        exit(1);
    }


    // if( (portnumber = atoi(argv[1])) < 0){
    //     fprintf(stderr,"change portnumber faild\n");
    //     exit(1);
    // }


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

    /*
        init save user data array
    */
    for(int i = 0;i < MAXUSERNUM; i++){
        online[i].fd = -1;
        online[i].flag = -1;
    }

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