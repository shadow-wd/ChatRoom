#ifndef OPERA_SERVER_H
#define OPERA_SERVER_H

#include "log.h"
#include "chatroom.h"


/*
    define online user array 
    max user number 32
*/
struct ONLINE online[MAXUSERNUM];



int  opera_register(int sockfd,int *index,struct protocol*msg);
void opera_login(int sockfd,int *index,struct protocol*msg);
void opera_offline(int index);




#endif