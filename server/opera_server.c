#include "opera_server.h"


/*
    find user exist or not
    return -1 no find user
    return other user index
*/
static int opera_finduser_register(char *name)
{
 int i;
 
 for(i=0;i<MAXUSERNUM;i++)
 {
    /*

    */
  if(online[i].flag == -1)
  {
   continue;
  }

  if(strcmp(name,online[i].name)==0)
  {
   return i;   
  }
 }
 return -1;
}


/*

*/
void opera_offline(int index)
{
    if(index < 0)
    {
        return;
    }
    online[index].fd = -1;
}


/*
    user loged
    return:
        NAME_PWD_NMATCH
        USER_LOGED
        OP_OK
*/
static int opera_user_loged(int sockfd,int *index,struct protocol*msg){

    for(int i = 0; i < MAXUSERNUM; i++){

        if(online[i].flag == -1){
            continue;
        }
        // LOGI("msg.data = %s\n",msg->data);
        // LOGI("msg.name = %s\n",msg->name);
        // LOGI("online.data = %s\n",online[i].passwd);
        // LOGI("online.data = %s\n",online[i].name);
        if( (strcmp(msg->name,online[i].name)== 0) &&(strcmp(msg->data,online[i].passwd) == 0) ){
            if(online[i].fd == -1){
                online[i].fd = 1;
                return OP_OK;
            }else{
                printf("this user had loged\n");
                return USER_LOGED;
            }
        }
    }
    return NAME_PWD_NMATCH;
}
/*
    add user
    return array index
*/
static int opera_adduser(int sockfd,struct protocol *msg)
{
    int i,index = -1;
 
    for(i=0;i<MAXUSERNUM;i++){
        if(online[i].flag == -1)
        {
            online[i].flag= 1;
            strcpy(online[i].name,msg->name);
            strcpy(online[i].passwd,msg->data);
            LOGI("regist name = %s to array = %d \n",msg->name,i);
            index = i;
            return index;
        }
    }
    return index;
}


/*
    user register

*/
int  opera_register(int sockfd,int *index,struct protocol *msg){

    int find_index;
    struct protocol msg_back;

    msg_back.cmd = REGISTER; 

    // user exist or not
    find_index = opera_finduser_register(msg->name);

    //user nonexist this user can register
    if(find_index == -1)
    { 
        *index = opera_adduser(sockfd,msg);
        LOGI("register return index = %d\n",*index);
        online[*index].flag = 1;
        msg_back.status = OP_OK;
  
        LOGI("user %s regist success!\n",msg->name);
        write(sockfd,&msg_back,sizeof(msg_back));
        printf("%s:%d:index = %d\n",__func__,__LINE__,*index);
        return;
    }else{
        msg_back.status = NAME_EXIST;
        LOGI("user %s exist!\n",msg->name);

        write(sockfd,&msg_back,sizeof(msg_back));
        return;
    } 

}


void opera_login(int sockfd,int *index,struct protocol *msg){
    int ret;
    struct protocol msg_back;

    msg_back.cmd = LOGIN; 
    ret = opera_user_loged(sockfd,index,msg);

    if(ret != OP_OK){
        msg_back.status = NAME_EXIST;
        LOGI("login fail!\n");
    }else{
        msg_back.status = OP_OK;
    }
    
    /*
    online[* index].fd is fail
    if user no register ,fd = -1.
    write function is failed
    */
   LOGI("login write function\n");
    write(sockfd,&msg_back,sizeof(msg_back));
}