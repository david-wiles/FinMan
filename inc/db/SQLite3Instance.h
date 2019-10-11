#ifndef FINMAN_SQLITE3_H
#define FINMAN_SQLITE3_H


#include <string>
#include <vector>
#include "AbstractDBInstance.h"
#include "QueryResult.h"
#include "AbstractQueryBuilder.h"
#include <sqlite3.h>


class SQLite3Instance : public AbstractDBInstance
{
public:
    explicit SQLite3Instance(const std::string& db_file);

    /**
     * Get a reference to the sqlite3 instance currently in use
     * @return SQLite3 object initialized to the proper database
     */
    static AbstractDBInstance* getInstance();

    /**
     * Execute commands to ensure that the proper tables are present in the database
     */
    void init_db() override;

    /**
     * Convenience function for executing a query with a prepared statement.  The SQL string should contain question
     * marks (?#) as placeholders for the values to be inserted into the prepared statement.
     *
     * @param sql       The SQL statement to be executed, with ?'s as placeholders
     * @param params    An array (vector) of parameters to replace ?'s in the SQL statement. The index of the parameter
     *                  should correspond to the index of the placeholder
     * @return          A DB result object containing column names returned and rows returned
     */
    QueryResult* query(std::string sql, std::vector<std::string> *params) override;

    /**
     * Execute a query from a QueryBuilder object
     *
     * @param query QueryBuilder object reference
     * @return      QueryResult returned from database
     */
    QueryResult* query(AbstractQueryBuilder* builder) override;

    ~SQLite3Instance() override;

private:
    SQLite3Instance();

    // The instance of a sqlite3 struct to be shared across the process
    sqlite3* db_instance;
    //Instance of error message to be referenced in queries
    char* err;
};


#endif //FINMAN_SQLITE3_H
