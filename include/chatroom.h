#ifndef CHATROOM_H
#define CHATROOM_H

// port number
#define SERVERPORT 8888



/*
    procotol.cmd 
*/
#define BROADCAST 0X00000001
#define PRIVATE 0X00000002
#define REGISTER 0X00000004
#define LOGIN 0X00000008
#define ONLINEUSER     0X00000010
#define LOGOUT     0X00000020


//  max ueser number
#define MAXUSERNUM 10

// max user name length
#define MAXUSERLEN 10

/* 
    cmd 
    status    communication status
    name     register and login  myself name
             private    target name
*/
struct protocol{
    int cmd;
    int status;
    char name[32];
    char data[64];
};


typedef struct {
    int userCount;
    char names[MAXUSERNUM][MAXUSERLEN];
} UserData;

/*
    protocol.status
    NAME_PWD_NMATCH 没有匹配的用户
    USER_LOGED      用户已登录
    ONLINEUSER_OK      show online user
    ONLINEUSER_OVER    online user translant over
*/
#define OP_OK            0X80000000
#define ONLINEUSER_OK    0X80000001
#define ONLINEUSER_OVER  0X80000002
#define NAME_EXIST       0X80000003
#define NAME_PWD_NMATCH  0X80000004
#define USER_LOGED       0X80000005
#define USER_NOT_REGIST  0X80000006

#endif