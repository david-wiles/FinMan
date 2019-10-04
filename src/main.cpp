#include <shell/Shell.h>
#include <shell/Auth.h>
#include <db/SQLite3.h>


int main()
{
    // Initialize database
    SQLite3* database = SQLite3::getInstance();
    database->init_db();

    // Authenticate user
    Auth::authenticate(database);

    // Begin main loop
    Shell::loop(Auth::username);

    // Close database connection
    delete database;

    return 0;
}
