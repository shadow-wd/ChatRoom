#ifndef OPERA_SERVER_H
#define OPERA_SERVER_H

#include "log.h"
#include "chatroom.h"
#include "../database/opera_db.h"


sqlite3 *db;

int  opera_register(int sockfd,struct protocol*msg);
void opera_login(int sockfd,struct protocol*msg);
void opera_offline(char *name);
void opera_show_online(int sockfd);

void opera_logout(int sockfd);


#endif