#ifndef FINMAN_DB_BASE_H
#define FINMAN_DB_BASE_H


#include "AbstractQueryBuilder.h"
#include "QueryResult.h"

/**
 * Database Instance
 *
 * Each subclass should at least implement init_db() and execute(). Init_db should check if the database has all
 * required tables, and create them if they don't exist.  Execute() should take a SQL string and a vector of string
 * parameters to execute a query, and then return the result as a DB_Result object.
 */
class AbstractDBInstance
{
public:
    // getInstance();
    virtual void init_db() = 0;

    /**
     * Execute a SQL statement from a prepared statement.
     *
     * @param sql       SQL text to query
     * @param params    Parameters to replace placeholders with
     * @return          QueryResult object with returned rows
     */
    virtual QueryResult* query(std::string sql, std::vector<std::string> *params) = 0;

    /**
     * Execute a SQL statement from a SQL builder class
     *
     * @param builder   The builder containg the SQL statement and values to prepare it with
     * @return          QueryResult of returned rows
     */
    virtual QueryResult* query(AbstractQueryBuilder* builder) = 0;

    virtual ~AbstractDBInstance() = default;

protected:
    AbstractDBInstance() = default;

    // Singleton instance
    static AbstractDBInstance* _instance;
};

#endif //FINMAN_DB_BASE_H
