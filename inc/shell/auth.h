#ifndef FINMAN_AUTH_H
#define FINMAN_AUTH_H

#include "../../SQLite/sqlite3.h"

void authenticate(sqlite3* db, char* username);
char* existing_user(sqlite3* db);
char* new_user(sqlite3* db);

#endif //FINMAN_AUTH_H
