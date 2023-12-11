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

    if(db_show(&db,&userdata) != 0){
        return;
    }
    printf("usercount =%d\n",userdata.userCount);
    for(int i = 0;i < userdata.userCount - 1;i++){
        strcpy(msg_back.name, userdata.names[i]);
        write(sockfd,&msg_back,sizeof(msg_back));
    }
    
    msg_back.cmd = ONLINEUSER_OVER;
    strcpy(msg_back.name, userdata.names[userdata.userCount - 1]);
    write(sockfd,&msg_back,sizeof(msg_back));
    // for (int j = 0; j < userdata.userCousnt; j++) {
    //     LOGI("User: %s\n", userdata.names[j]);
    // }
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