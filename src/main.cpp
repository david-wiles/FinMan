#include <shell/Shell.h>
#include <shell/Auth.h>
#include <db/SQLite3Instance.h>


int main()
{
    // Initialize database
    AbstractDBInstance* database = SQLite3Instance::getInstance();
    database->init_db();

    // Begin main loop
    Shell shell(Auth::authenticate());
    shell.loop();

    // Close database connection
    delete database;

    return 0;
}
