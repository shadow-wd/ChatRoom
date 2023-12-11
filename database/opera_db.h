#ifndef OPERA_DB_H
#define OPERA_DB_H
#include "sqlite3.h"
#include <stdbool.h>
#include <stdio.h>
#include "stdlib.h"
#include "chatroom.h"
#include "log.h"





/*
user
Table
---------------------------------
fd      flag      NAME       passwd
---------------------------------

*/

int db_init(sqlite3 **db);


/*
    register user   
    sqlite3 **db,char* name,int fd,int flag,char* passwd
*/
int db_register(sqlite3 **db,char *name ,char *passwd,int fd,int flag);



 /*
    set online or offline
    sqlite3 **db, int fd
 */
int db_line(sqlite3 **db,char *name,int fd);


int db_modifyfd(sqlite3 **db,int fd,int new_fd);

 /*
    sqlite3 **db
 */
int db_isonline(sqlite3 **db,char *name);


 /*
    sqlite3 **db
 */
int db_isregister(sqlite3 **db,char *name);


/*
    show online user
*/
int db_show(sqlite3 **db ,UserData *userdata);


int db_login(sqlite3 **db,char *name,char *passwd,int fd);
/*  
    sqlite3 **db
*/
int db_close(sqlite3 **db );



#endif