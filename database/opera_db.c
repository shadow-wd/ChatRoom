#include "opera_db.h"


static int callback(void *NotUsed, int argc, char **argv, char **azColName){

    return 0;  
}


static int callback_login(void *NotUsed, int argc, char **argv, char **azColName){
    if (argc == 1 && argv[0] != NULL) {
        char **ispasswd = (char **)NotUsed;

        *ispasswd = strdup(argv[0]);

        return 0;  // 返回 0 表示继续处理下一行
    }
    printf("111\n");
    return 0;
}

static int callback_isregister(void *NotUsed, int argc, char **argv, char **azColName){
    int *nameExists = (int *)NotUsed;

    // 如果查询结果中有一行，则表示数据库中存在这个名字
    *nameExists = (argc > 0 && argv[0] != NULL);
    return 0;  
}


static int callback_isonline(void *NotUsed, int argc, char **argv, char **azColName){
    int *fdValue = (int *)NotUsed;

    // 确保至少有一列，并且该列的值不为 NULL
    if (argc > 0 && argv[0] != NULL) {
        // 将字符串表示的 FD 转换为整数，并存储在 *fdValue 中
        *fdValue = atoi(argv[0]);
    }
    return 0;
}


static int callback_show(void *userData, int argc, char **argv, char **azColName){
    UserData *data = (UserData *)userData;

    // 如果有名字，将其存储到 UserData 结构中
    if (argc > 0 && argv[0] != NULL && data->userCount < MAXUSERNUM) {
        strncpy(data->names[data->userCount], argv[0], MAXUSERLEN - 1);
        data->names[data->userCount][MAXUSERLEN - 1] = '\0';
        (data->userCount)++;
    }

    return 0;
}


/*
    create table
    fd    name   passwd    flag
*/
static int db_create(sqlite3 **db){
    const char * sql;
    char *errmsg= NULL;
    sql = "CREATE TABLE IF NOT EXISTS USER(" \
    "FD    INT             NOT NULL," \
    "FLAG    INT             NOT NULL," \
    "NAME   TEXT            NOT NULL," \
    "PASSWD   TEXT            NOT NULL);";

    if( sqlite3_exec(*db,sql,callback,0,errmsg) != SQLITE_OK){
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }else{
        fprintf(stdout,"Table create Successfully!\n");
    }
    return 0;
}


/*
  init data to table  
*/
int db_init_table(sqlite3 **db){
    const char * sql;
    char *errmsg= NULL;
    sql = "INSERT OR IGNORE INTO USER (FD,FLAG,NAME,PASSWD) "  \
         "VALUES (1, 1,'Paul', 'SS'); " \
         "INSERT OR IGNORE INTO USER (FD,FLAG,NAME,PASSWD)" \
         "VALUES (1,-1, 'Mark', '25');";
    LOGI("cmd:%s\n",sql);
    if( sqlite3_exec(*db,sql,callback,0,errmsg) != SQLITE_OK){
        fprintf(stderr, "SQL add error: %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }else{
        fprintf(stdout,"Records create Successfully!\n");
    }
    return 0;
}


/* 
    show online user  fd =1
*/
int db_show(sqlite3 **db ,UserData *userdata){
    const char * sql;
    char *errmsg= NULL;

    sql = "SELECT name FROM user WHERE FD > 0;";


    if( sqlite3_exec(*db,sql,callback_show,userdata,errmsg) != SQLITE_OK){
        fprintf(stderr, "SQL show fail %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }else{
        fprintf(stdout,"SQL show Successfully!\n");
    }

    // for (int i = 0; i < userdata->userCount; i++) {
    //     printf("User: %s\n", userdata->names[i]);
    // }


    return 0;
}

/*
    delete the data in the table
*/
int db_delete(sqlite3 **db ){
    // const char * sql;
    char sql[100];
    char *errmsg= NULL;
    int id;

    printf("Enter the ID of the data you want to delete:");
    scanf("%d", &id);
    
    // id nonexist or query fail
    // if(isIdExists_rb(db,id) != 1){
    //     printf("id nonexist\n");
    //     return 0;
    // }

    sprintf(sql,"DELETE from COMPANY where ID=%d;", id);
    LOGI("cmd:%s\n",sql);
    if( sqlite3_exec(*db,sql,callback,0,errmsg) != SQLITE_OK){
        fprintf(stderr, "SQL delete fail %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }else{
        fprintf(stdout,"SQL delete Successfully!\n");
    }

    return 0;
}



/*
    user online or offline
    fd -1 offline   1 online
    return 0 successful    -1 fail
*/
int db_line(sqlite3 **db,char *name,int fd){
    char sql[100];
    char *errmsg= NULL;

    sprintf(sql,"UPDATE USER set FD= %d where NAME='%s';",fd, name);

    LOGI("cmd:%s\n",sql);
    if( sqlite3_exec(*db,sql,NULL,0,errmsg) != SQLITE_OK){
        fprintf(stderr, "SQL update fail %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }else{
        fprintf(stdout,"SQL update Successfully!\n");
    }
    return 0;
}

/*
    user online or offline
    fd -1 offline   1 online
    return 0 successful    -1 fail
*/
int db_modifyfd(sqlite3 **db,int fd,int new_fd){
    char sql[100];
    char *errmsg= NULL;

    sprintf(sql,"UPDATE USER set FD= %d where FD=%d;",new_fd, fd);

    LOGI("cmd:%s\n",sql);
    if( sqlite3_exec(*db,sql,NULL,0,errmsg) != SQLITE_OK){
        fprintf(stderr, "fd update fail %s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }else{
        fprintf(stdout,"fd update Successfully!\n");
    }
    return 0;
}


/*
    user online or offline
    fd -1 offline   1 online
    return  successful 0   fail -1
*/
int db_register(sqlite3 **db,char *name ,char *passwd,int fd,int flag){
    char sql[200];
    char *errmsg= NULL;
    int ret;


    snprintf(sql, sizeof(sql), "INSERT INTO USER (FD, FLAG, NAME, PASSWD) VALUES (%d, %d, '%s', '%s');", fd, flag, name, passwd);
    LOGI("cmd:%s\n",sql);
    
    if( (ret = sqlite3_exec(*db,sql,NULL,0,errmsg)) != SQLITE_OK){
        fprintf(stderr, "SQL show fail %s\n", errmsg);
        printf("ret = %d\n",ret);
        sqlite3_free(errmsg);
        return -1;
    }else{
        fprintf(stdout,"SQL show Successfully!\n");
    }
    return 0;
}


/*
    online or offline
    return 1 online      0  offline 
*/
int db_isonline(sqlite3 **db, char *name) {
    char sql[200];
    char *errmsg = NULL;
    int ret;
    int fd;

    snprintf(sql, sizeof(sql), "SELECT FD FROM USER WHERE NAME = '%s';", name);
    LOGI("cmd: %s\n", sql);


    ret = sqlite3_exec(*db, sql, callback_isonline,&fd, &errmsg);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "query online status fail err=%s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    }
    sqlite3_free(errmsg);
    if(fd == 1){
        return 1;
    }
    return 0;
}


int db_isregister(sqlite3 **db,char *name){
    char sql[200];
    char *errmsg = NULL;
    int ret;
    int nameisexit = 0;

    snprintf(sql, sizeof(sql), "SELECT 1 FROM USER WHERE NAME = '%s' LIMIT 1;", name);
    LOGI("cmd: %s\n", sql);


    ret = sqlite3_exec(*db, sql, callback_isregister, &nameisexit, &errmsg);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "query register status fail err=%s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    } 
    sqlite3_free(errmsg);
    return nameisexit;
}


int db_login(sqlite3 **db,char *name,char *passwd,int fd){
    char sql[200];
    char *errmsg = NULL;
    int ret;
    char *ispasswd =NULL;

    snprintf(sql, sizeof(sql), "SELECT PASSWD FROM USER WHERE NAME = '%s';", name);
    LOGI("cmd: %s\n", sql);


    ret = sqlite3_exec(*db, sql, callback_login, &ispasswd, &errmsg);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "query register status fail err=%s\n", errmsg);
        sqlite3_free(errmsg);
        return -1;
    } 

    if (ispasswd != NULL && strcmp(passwd, ispasswd) == 0) {
        printf("Login successful!\n");
        db_line(db,name,fd);
        sqlite3_free(errmsg);
        // sqlite3_free(ispasswd);
        return 0;
    } else {
        printf("Login failed!\n");
        sqlite3_free(errmsg);
        // sqlite3_free(ispasswd);
        return -1;
    }
}




int db_close(sqlite3 **db ){
    if(sqlite3_close(*db) != SQLITE_OK){
        fprintf(stderr,"close database fail!\n");   
    }else{
        printf("close database Successfully!\n");
    };
}



/*  
    return 0  successful    -1 fail
*/
int db_init(sqlite3 **db){
    if( sqlite3_open("user.db",db) != 0){
        fprintf(stdout,"open database fail!\n");
        return -1;
    }
    printf("database open Successfully!\n");
        if(db_create(db) == -1){
        return -1;
    }
    // db_init_table(db);

    return 0;
}