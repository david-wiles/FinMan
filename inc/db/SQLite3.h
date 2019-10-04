#ifndef FINMAN_SQLITE3_H
#define FINMAN_SQLITE3_H


#include <string>
#include <vector>
#include "DB_Base.h"
#include <sqlite3.h>


class SQLite3 : public DB_Base
{
public:
    /**
     * Get a reference to the sqlite3 instance currently in use
     * @return SQLite3 object initialized to the proper database
     */
    static SQLite3* getInstance();

    /**
     * Execute commands to ensure that the proper tables are present in the database
     */
    void init_db() override;

    /**
     * Convenience function for executing a query with a prepared statement.  The SQL string should contain question
     * marks (?) as placeholders for the values to be inserted into the prepared statement.
     *
     * @param sql       The SQL statement to be executed, with ?'s as placeholders
     * @param params    An array (vector) of parameters to replace ?'s in the SQL statement. The index of the parameter
     *                  should correspond to the index of the placeholder
     * @return          A 2-d array of rows and columns returned by the query. The first row will always be the column
     *                  names, even if no data was returned.
     */
    std::vector<std::vector<std::string>>* execute(std::string sql, std::vector<std::string> *params);

    /**
     * Close the database connection and free all memory associated with the database instance
     */
    ~SQLite3() override;

private:
    /**
     * Private constructor which opens a database connection
     */
    SQLite3();

    // The instance of the database object
    static SQLite3* instance;
    // The instance of a sqlite3 struct to be shared across the process
    sqlite3* db_instance;
    //Instance of error message to be referenced in queries
    char* err;
};


#endif //FINMAN_SQLITE3_H
