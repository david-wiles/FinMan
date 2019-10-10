#include <shell/Shell.h>
#include <shell/Auth.h>
#include <db/SQLite3.h>


int main()
{
    // Initialize database
    DB_Base* database = SQLite3::getInstance();
    database->init_db();

    // Begin main loop
    Shell shell(Auth::authenticate());
    shell.loop();

    // Close database connection
    delete database;

    return 0;
}
