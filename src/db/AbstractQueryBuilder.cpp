#include "db/AbstractQueryBuilder.h"


void AbstractQueryBuilder::select(std::vector<std::string> cols)
{
    _type = QueryType::SELECT;
    _cols = std::move(cols);
}

void AbstractQueryBuilder::insert(std::vector<std::string> cols)
{
    _type = QueryType::INSERT;
    _cols = std::move(cols);
}

void AbstractQueryBuilder::update(std::vector<std::pair<std::string, std::string>> conds)
{
    _type = QueryType::UPDATE;
    _set = std::move(conds);
}

void AbstractQueryBuilder::update(std::pair<std::string, std::string> cond)
{
    _type = QueryType::UPDATE;
    _set = {std::move(cond)};
}

void AbstractQueryBuilder::where(std::vector<std::pair<std::string, std::string>> where)
{
    _where = std::move(where);
}

void AbstractQueryBuilder::where(std::pair<std::string, std::string> where)
{
    _where = {std::move(where)};
}

void AbstractQueryBuilder::values(std::vector<std::vector<std::string>> vals)
{
    if (_type == QueryType::INSERT)
        _inserts = std::move(vals);
}

