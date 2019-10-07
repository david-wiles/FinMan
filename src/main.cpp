#include <shell/Shell.h>
#include <shell/Auth.h>
#include <db/SQLite3.h>


int main()
{
    // Initialize database
    SQLite3* database = SQLite3::getInstance();

    database->init_db();

    // Authenticate user
    Auth auth(database);

    auth.authenticate();

    // Begin main loop
    Shell shell(auth.get_username());

    shell.loop();

    // Close database connection
    delete database;

    return 0;
}
