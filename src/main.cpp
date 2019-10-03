#include <shell/Shell.h>
#include <shell/Auth.h>

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

        Auth::authenticate(db);

        Shell::loop(Auth::username);

        sqlite3_close(db);
    }

    return 0;
}
