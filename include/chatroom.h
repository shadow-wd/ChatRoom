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
#define MAXUSERNUM 32


/*
    online user struct
    fd      -1 or 1   online flag   -1 :user offline  1:user online
    flag    register or not     -1 :no register     1: user register
    name 
    passwd
*/
struct ONLINE{
    int fd;
    int flag;
    char name[32];
    char passwd[32];
};


/* 
    cmd 
    status    communication status
*/
struct protocol{
    int cmd;
    int status;
    char name[32];
    char data[64];
};


/*
    protocol.status
    NAME_PWD_NMATCH 没有匹配的用户
    USER_LOGED      用户已登录
*/
#define OP_OK            0X80000000
#define ONLINEUSER_OK    0X80000001
#define ONLINEUSER_OVER  0X80000002
#define NAME_EXIST       0X80000003
#define NAME_PWD_NMATCH  0X80000004
#define USER_LOGED       0X80000005
#define USER_NOT_REGIST  0X80000006

#endif