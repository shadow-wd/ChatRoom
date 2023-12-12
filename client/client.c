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
#include "opera_client.h"


#define BUFFERSIZE 1024


login_flag = -1;


void *rec_func(void *arg){
    char buffer[BUFFERSIZE];
    int nbytes;
    struct protocol msg;
    int fd = *(int *)arg;
    free(arg);
    while(1){
        if(login_flag != 1){
            continue;
        }
        nbytes = read(fd,&msg,sizeof(msg));;
        if(nbytes == -1){
            close(fd);
            break;
        }else if(nbytes == 0){
            close(fd);
            printf("server disconnect!\n");
            break;
        }
        printf("Message#%s\n",msg.data);
    }

}

int main(int argc,char *argv[]){
    int sockfd;
    struct sockaddr_in server_addr;
    int *pthreadarg = NULL;
    pthread_t thread;
    int choose;
    int max_sel,min_sel;

    if(argc != 2){
        fprintf(stderr,"input invalid!\n");
        exit(1);
    }


    // if( (portnumber = atoi(argv[2])) < 0){
    //     fprintf(stderr,"change portnumber faild\n");
    //     exit(1);
    // }

    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        fprintf(stderr,"socket create failed!\n");
        exit(1);
    }    


    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_port = htons(SERVERPORT);
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
    system("clear");
    while(1){
        if(login_flag == -1){
            printf("1. login\n");
            printf("2. register\n");
        }else{
            printf("3. private\n");
            printf("4. public chat\n");
            printf("5. online list\n");
            printf("6. login out\n");
        }

        printf("input send mesggage:\n");
        if(scanf("%d",&choose) != 1){
            while (getchar() != '\n');
        }
        if(login_flag == 1){
            max_sel = 6;
            min_sel = 3;
        }else if(login_flag == -1){
            max_sel = 2;
            min_sel = 1;
        }
        // LOGI("max = %d, choose = %d, min = %d\n",max_sel,choose,min_sel);
        if(choose < min_sel || choose > max_sel){
            fprintf(stderr,"input invaild!\n");
            continue;
        }
        switch (choose)
        {
        case 1:
            opera_login(sockfd);
            break;
        case 2:
            opera_register(sockfd);
            break;
        case 3:
            opera_private(sockfd);
            break;
        case 4:
            opera_broadcast(sockfd);
            break;
        case 5:
             opera_show_online(sockfd);
            break;
        case 6:
            opera_close(sockfd);
            break;
        default:
            break;
        }

    }
    pthread_join(thread, NULL);
    return 0;

}