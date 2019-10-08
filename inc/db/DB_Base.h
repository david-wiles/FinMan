#ifndef FINMAN_DB_BASE_H
#define FINMAN_DB_BASE_H


#include "DB_Result.h"

/**
 * Base class for a connection to a database. DB_Base classes should be singletons, and so should all implement their
 * own static getInstance() method for getting the connection to the database.
 *
 * Each subclass should at least implement init_db() and execute(). Init_db should check if the database has all
 * required tables, and create them if they don't exist.  Execute() should take a SQL string and a vector of string
 * parameters to execute a query, and then return the result as a DB_Result object.
 */
class DB_Base
{
public:
    // getInstance();
    virtual void init_db() = 0;
    virtual DB_Result* execute(std::string sql, std::vector<std::string> *params) = 0;
    virtual ~DB_Base() { delete(_instance); };

protected:
    DB_Base() = default;
    static DB_Base* _instance;
};

#endif //FINMAN_DB_BASE_H
