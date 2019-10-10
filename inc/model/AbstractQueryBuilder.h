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
class AbstractQueryBuilder
{
public:
    // getInstance();
    virtual void init_db() = 0;

    // CRUD
    virtual bool createRow(std::string table, std::vector<std::pair<std::string, std::string>>* kv_insert) = 0;
    virtual QueryResult* findRow(std::string table, std::vector<std::string>* cols,
            std::vector<std::pair<std::string, std::string>>* kv_where) = 0;
    virtual bool updateRow(std::string table,
            std::vector<std::pair<std::string, std::string>>* kv_update,
            std::vector<std::pair<std::string, std::string>>* kv_where) = 0;
    virtual bool deleteRow(std::string table, std::vector<std::pair<std::string, std::string>>* kv_where) = 0;

    virtual QueryResult* execute(std::string sql, std::vector<std::string> *params) = 0;

    virtual ~AbstractQueryBuilder() { delete(_instance); };

protected:
    AbstractQueryBuilder() = default;

    static AbstractQueryBuilder* _instance;
};

#endif //FINMAN_DB_BASE_H
