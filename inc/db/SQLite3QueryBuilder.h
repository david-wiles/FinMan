#ifndef FINMAN_SQLITE3QUERYBUILDER_H
#define FINMAN_SQLITE3QUERYBUILDER_H


#include "AbstractQueryBuilder.h"

class SQLite3QueryBuilder : public AbstractQueryBuilder
{
public:
    void build() override;
};


#endif //FINMAN_SQLITE3QUERYBUILDER_H
