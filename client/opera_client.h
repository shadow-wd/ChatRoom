#ifndef OPERA_CLIENT_H
#define OPERA_CLIENT_H

#include "log.h"
#include "chatroom.h"
#include <unistd.h>
/*  
    user login status
*/
extern int login_flag;


void opera_close(int sockfd);
int opera_login(int sockfd);
int opera_register(int sockfd);


int opera_private(int fd);

int opera_broadcast(int sockfd);


int opera_show_online(int sockfd);
#endif