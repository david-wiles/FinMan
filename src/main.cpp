#include <shell/Shell.h>
#include <shell/Auth.h>
#include <model/SQLite3DB.h>


int main()
{
    // Initialize database
    AbstractDB* database = SQLite3DB::getInstance();
    database->init_db();

    // Begin main loop
    Shell shell(Auth::authenticate());
    shell.loop();

    // Close database connection
    delete database;

    return 0;
}
