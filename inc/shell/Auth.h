#ifndef FINMAN_AUTH_H
#define FINMAN_AUTH_H

#include "../../SQLite/sqlite3.h"

namespace Auth
{
    void authenticate(sqlite3* db);

    void existing_user(sqlite3 *db);

    void new_user(sqlite3 *db);

    void set_username(char* name);

    char* username = nullptr;
}

#endif //FINMAN_AUTH_H
