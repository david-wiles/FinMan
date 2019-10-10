#ifndef FINMAN_SQLITE3_H
#define FINMAN_SQLITE3_H


#include <string>
#include <vector>
#include "AbstractQueryBuilder.h"
#include "QueryResult.h"
#include <sqlite3.h>


class SQLite3QueryBuilder : public AbstractQueryBuilder
{
public:
    explicit SQLite3QueryBuilder(std::string db_file);

    /**
     * Get a reference to the sqlite3 instance currently in use
     * @return SQLite3 object initialized to the proper database
     */
    static AbstractQueryBuilder* getInstance();

    /**
     * Execute commands to ensure that the proper tables are present in the database
     */
    void init_db() override;

    // CRUD
    bool createRow(std::string table, std::vector<std::pair<std::string, std::string>>* kv_insert) override;
    QueryResult* findRow(std::string table, std::vector<std::string>* cols, std::vector<std::pair<std::string, std::string>>* kv_where) override;
    bool updateRow(std::string table, std::vector<std::pair<std::string, std::string>>* kv_update, std::vector<std::pair<std::string, std::string>>* kv_where) override;
    bool deleteRow(std::string table, std::vector<std::pair<std::string, std::string>>* kv_where) override;

    /**
     * Convenience function for executing a query with a prepared statement.  The SQL string should contain question
     * marks (?#) as placeholders for the values to be inserted into the prepared statement.
     *
     * @param sql       The SQL statement to be executed, with ?'s as placeholders
     * @param params    An array (vector) of parameters to replace ?'s in the SQL statement. The index of the parameter
     *                  should correspond to the index of the placeholder
     * @return          A DB result object containing column names returned and rows returned
     */
    QueryResult* execute(std::string sql, std::vector<std::string> *params) override;

    ~SQLite3QueryBuilder() override;

private:
    SQLite3QueryBuilder();

    // The instance of a sqlite3 struct to be shared across the process
    sqlite3* db_instance;
    //Instance of error message to be referenced in queries
    char* err;
};


#endif //FINMAN_SQLITE3_H