#include "opera_server.h"



/*
    user offline
*/
void opera_offline(char *name)
{

    db_line(&db,name,-1);

}


/*
    user register

*/
int  opera_register(int sockfd,struct protocol *msg){

    struct protocol msg_back;

    msg_back.cmd = REGISTER; 


    if(db_register(&db,msg->name,msg->data,-1,-1) == 0){
        LOGI("user %s regist success!\n",msg->name);
        msg_back.status = OP_OK;
        write(sockfd,&msg_back,sizeof(msg_back));
        return ;
    }
    msg_back.status = NAME_EXIST;
    write(sockfd,&msg_back,sizeof(msg_back));
    return ;

}


/*
    user login
*/
void opera_login(int sockfd,struct protocol *msg){
    int ret;
    struct protocol msg_back;

    msg_back.cmd = LOGIN; 

    ret = db_login(&db,msg->name,msg->data,sockfd);

    if(ret != 0){
        msg_back.status = NAME_EXIST;
        LOGI("login fail!\n");
    }else{
        msg_back.status = OP_OK;
    }
    
   LOGI("login write function\n");
    write(sockfd,&msg_back,sizeof(msg_back));
}


/*
    show all online user name
*/
void opera_show_online(int sockfd){
    struct protocol msg_back;
    UserData userdata;
    userdata.userCount = 0;

    msg_back.cmd = ONLINEUSER; 
    msg_back.status = ONLINEUSER_OK;
    if(db_show(&db,&userdata) != 0){
        return;
    }
    printf("usercount =%d\n",userdata.userCount);
    for(int i = 0;i < userdata.userCount - 1;i++){
        strcpy(msg_back.name, userdata.names[i]);
        write(sockfd,&msg_back,sizeof(msg_back));
    }
    
    msg_back.status = ONLINEUSER_OVER;
    strcpy(msg_back.name, userdata.names[userdata.userCount - 1]);
    write(sockfd,&msg_back,sizeof(msg_back));
    // for (int j = 0; j < userdata.userCousnt; j++) {
    //     LOGI("User: %s\n", userdata.names[j]);
    // }
}


/*
    fd sender 
    msg->name  receiver
    user private chat
*/
int opera_private(int fd,struct protocol *msg){
    struct protocol msg_back;
    int sockfd;
    char name[100];
    // find reciever fd
    sockfd = db_finduserfd(&db,msg->name);

    // find sender name
    if(db_findusername(&db,fd,name) != 0){
        return -1;
    }
    printf("name sfafdafda sfdfddd= %s\n",name);
    if(!(sockfd>0)){
        return -1;
    }
    printf("name sfafdafda ddd= %s\n",name);
    msg_back.cmd = PRIVATE;

    snprintf(msg_back.data, sizeof(msg_back.data), "%s %s %s:%s", name,"send to",msg->name,msg->data);
    write(sockfd,&msg_back,sizeof(msg_back));
    return 0;
}



int opera_broadcast(int fd,struct protocol *msg){
    struct protocol msg_back;
    UserData userdata;
    userdata.userCount = 0;
    char name[100];
    int sockfd = -1;


    msg_back.cmd = BROADCAST; 
    // find sender name
    if(db_findusername(&db,fd,name) != 0){
        return -1;
    }


    // find all online user fd and name
    if(db_show(&db,&userdata) != 0){
        return;
    }
    LOGI("usercount = %d\n",userdata.userCount);

    // strcpy(msg_back.data, msg->data);
    snprintf(msg_back.data, sizeof(msg_back.data), "%s say: %s", name,msg->data);
    for(int i = 0;i < userdata.userCount;i++){
        strcpy(msg_back.name, userdata.names[i]);
        sockfd = db_finduserfd(&db,msg_back.name);
        if(sockfd != fd){
            LOGI("%s send to %s\n",name,msg_back.name);
            LOGI("%d send to %d\n",fd,sockfd);
            write(sockfd,&msg_back,sizeof(msg_back));
        }
    }
    return 0;
}
  
/*
     logout
*/
void opera_logout(int sockfd){
    int ret;
    struct protocol msg_back;

    msg_back.cmd = LOGOUT; 

    ret = db_modifyfd(&db,sockfd,-1);

    if(ret != 0){
        msg_back.status = NAME_EXIST;
        LOGI("logout fail!\n");
    }else{
        msg_back.status = OP_OK;
    }

    write(sockfd,&msg_back,sizeof(msg_back));
}