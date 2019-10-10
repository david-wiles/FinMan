#ifndef FINMAN_DB_BASE_H
#define FINMAN_DB_BASE_H


#include "QueryResult.h"

/**
 * Database query builder class
 *
 * Each subclass should at least implement init_db() and execute(). Init_db should check if the database has all
 * required tables, and create them if they don't exist.  Execute() should take a SQL string and a vector of string
 * parameters to execute a query, and then return the result as a DB_Result object.
 */
class AbstractDB
{
public:
    // getInstance();
    virtual void init_db() = 0;

    virtual QueryResult* query(std::string sql, std::vector<std::string> *params) = 0;

    virtual ~AbstractDB() { delete(_instance); };

protected:
    AbstractDB() = default;

    static AbstractDB* _instance;
};

#endif //FINMAN_DB_BASE_H
