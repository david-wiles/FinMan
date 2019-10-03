#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

#include "shell/auth.h"


void authenticate(sqlite3* db, char* username)
{
    printf("Log in? (y/n), press n to create a new user.");

    if (getchar() == 'y') {
        username = existing_user(db);
    } else {
        username = new_user(db);
    }

}

static int auth_user(void *notUsed, int col_count, char **col_data, char **col_name)
{

    if (strcmp(col_data[0], (char*) '1') == 0);

    return 0;
}

char* existing_user(sqlite3* db)
{
    size_t max_length = 128;
    char *zErrMsg = NULL;
    char* sql = "SELECT COUNT(*) FROM auth_user WHERE username = ? AND password = ?;";

    printf("Username: ");
    char* input_username = NULL;
    getline(&input_username, &max_length, stdin);

    printf("Password: ");
    char* input_password = NULL;
    getline(&input_password, &max_length, stdin);


    unsigned char md5[MD5_DIGEST_LENGTH];
    MD5((unsigned char*) input_password, strlen(input_password), md5);

    sqlite3_exec(db, sql, auth_user, NULL, &zErrMsg);
}

static int create_user(void *NotUsed, int argc, char **argv, char **azColName)
{

}

char* new_user(sqlite3* db)
{

}