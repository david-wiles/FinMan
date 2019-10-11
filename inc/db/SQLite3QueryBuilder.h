#ifndef FINMAN_SQLITE3QUERYBUILDER_H
#define FINMAN_SQLITE3QUERYBUILDER_H


#include <utility>

#include "AbstractQueryBuilder.h"

/**
 * SQLite3 QueryBuilder
 *
 * Inherits all methods from Abstract QueryBuilder.  Calling build will create a basic statement of SQLite3 SQL
 */
class SQLite3QueryBuilder : public AbstractQueryBuilder
{
public:
    explicit SQLite3QueryBuilder(std::string table) : AbstractQueryBuilder(std::move(table)) {};

    void build() override;

    ~SQLite3QueryBuilder() override { delete(_vals); };

private:
    // Build the where condition from WHERE OR and WHERE AND
    std::string build_where(int val_index);

};


#endif //FINMAN_SQLITE3QUERYBUILDER_H
