#include "opera_client.h"

/*
    user register
*/
int opera_register(int sockfd){
    int ret;
    struct protocol msg,msg_back;

    msg.cmd = REGISTER; 
    printf("input your name\n"); 
    scanf("%s",msg.name);
    printf("input your passwd\n"); 
    scanf("%s",msg.data);

    if((write(sockfd,&msg,sizeof(msg))) == -1){
        fprintf(stderr,"send data fail!\n");
        close(sockfd);
        return;
    }
    if((read(sockfd,&msg_back,sizeof(msg_back))) == -1){
        fprintf(stderr,"read data fail!\n");
        close(sockfd);
        return;
    }

    if(msg_back.status != OP_OK){
        fprintf(stderr,"register fail!\n");
        login_flag = -1;
        return ;
    }

    return OP_OK;
}



/*
    user login
*/
int opera_login(int sockfd){
    int ret;
    struct protocol msg,msg_back;

    msg.cmd = LOGIN; 
    printf("input your name\n"); 
    scanf("%s",msg.name);
    printf("input your passwd\n"); 
    scanf("%s",msg.data);

    if((write(sockfd,&msg,sizeof(msg))) == -1){
        fprintf(stderr,"send data fail!\n");
        close(sockfd);
        return;
    }
    if((read(sockfd,&msg_back,sizeof(msg_back))) == -1){
        fprintf(stderr,"read data fail!\n");
        close(sockfd);
        return;
    }

    if(msg_back.status != OP_OK){
        fprintf(stderr,"login fail!\n");
        login_flag = -1;
    }else{
        fprintf(stdin,"login successfully!\n");
        login_flag = 1;
    }
    return OP_OK;
}



void opera_close(int fd){
    close(fd);
    login_flag = -1;

}
