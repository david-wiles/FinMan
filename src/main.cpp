#include <shell/Shell.h>
#include <shell/Auth.h>
#include <model/SQLite3QueryBuilder.h>


int main()
{
    // Initialize database
    AbstractQueryBuilder* database = SQLite3QueryBuilder::getInstance();
    database->init_db();

    // Begin main loop
    Shell shell(Auth::authenticate());
    shell.loop();

    // Close database connection
    delete database;

    return 0;
}
