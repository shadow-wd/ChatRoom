#ifndef OPERA_CLIENT_H
#define OPERA_CLIENT_H

#include "log.h"
#include "chatroom.h"
#include <unistd.h>
/*  
    user login status
*/
extern int login_flag;


void opera_close(int fd);
int opera_login(int sockfd);
int opera_register(int sockfd);

#endif