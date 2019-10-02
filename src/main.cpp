#include <shell/shell.h>
#include "../SQLite/sqlite3.h"


static int check_exists(void *NotUsed, int argc, char **argv, char **azColName) {

}

bool init_db(sqlite3* db)
{
    if (!sqlite3_open("data.db", &db))
        return false;
    char *zErrMsg = nullptr;
    const char* sql = "SELECT name FROM sqlite_master WHERE type='table';";
    sqlite3_exec(db, sql, check_exists, nullptr, &zErrMsg);

    return true;
}

int main()
{
    sqlite3* db = nullptr;

    if (init_db(db)) {

        shell_loop();

        sqlite3_close(db);
    }

    return 0;
}
