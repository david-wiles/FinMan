#ifndef FINMAN_SQLITE3QUERYBUILDER_H
#define FINMAN_SQLITE3QUERYBUILDER_H


#include <utility>

#include "AbstractQueryBuilder.h"

class SQLite3QueryBuilder : public AbstractQueryBuilder
{
public:
    explicit SQLite3QueryBuilder(std::string table) : AbstractQueryBuilder(std::move(table)) {};

    void build() override;

    ~SQLite3QueryBuilder() override { delete(_vals); };

private:
    std::string build_where(int val_index);

};


#endif //FINMAN_SQLITE3QUERYBUILDER_H
