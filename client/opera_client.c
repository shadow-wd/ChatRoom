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




int opera_show_online(int sockfd){
    int ret;
    struct protocol msg,msg_back;
    UserData userdata;
    userdata.userCount = 0;

    msg.cmd = ONLINEUSER; 
    

    if((write(sockfd,&msg,sizeof(msg))) == -1){
        fprintf(stderr,"online show fail!\n");
        close(sockfd);
        return;
    }

    // while (1) {
    //     // 读取传输的数据
    //     ssize_t bytesRead = read(sockfd, &msg_back, sizeof(msg_back));
    //     strcpy(userdata.names[userdata.userCount], msg_back.name);
    //     userdata.userCount++;
    //     if (msg_back.cmd == ONLINEUSER_OVER) {
    //         break;
    //     }

    // }
    // // printf("usercount =%d\n",userdata.userCount);
    // for (int i = 0; i < userdata.userCount; i++) {
    //     printf("User: %s\n", userdata.names[i]);
    // }
    return 0;
}



int opera_private(int sockfd){
    int ret;
    struct protocol msg,msg_back;

    msg.cmd = PRIVATE; 
    printf("input send to name:\n"); 
    scanf("%s",msg.name);
    printf("input message:\n"); 
    scanf("%s",msg.data);

    if((write(sockfd,&msg,sizeof(msg))) == -1){
        fprintf(stderr,"send data fail!\n");
        close(sockfd);
        return;
    }

    
    // if((read(sockfd,&msg_back,sizeof(msg_back))) == -1){
    //     fprintf(stderr,"read data fail!\n");
    //     close(sockfd);
    //     return;
    // }


}

int opera_broadcast(int sockfd){
    int ret;
    struct protocol msg,msg_back;

    msg.cmd = BROADCAST; 
    printf("input message:\n"); 
    scanf("%s",msg.data);

    if((write(sockfd,&msg,sizeof(msg))) == -1){
        fprintf(stderr,"send data fail!\n");
        close(sockfd);
        return;
    }


}



void opera_close(int sockfd){
        int ret;
    struct protocol msg,msg_back;

    msg.cmd = LOGOUT; 

    if((write(sockfd,&msg,sizeof(msg))) == -1){
        fprintf(stderr,"online show fail!\n");
        close(sockfd);
        return;
    }
    if((read(sockfd,&msg_back,sizeof(msg_back))) == -1){
        fprintf(stderr,"online show fail!\n");
        close(sockfd);
        return;
    }
    close(sockfd);
    return 0;

}
