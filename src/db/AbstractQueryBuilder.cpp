#include "db/AbstractQueryBuilder.h"


AbstractQueryBuilder* AbstractQueryBuilder::select(std::vector<std::string> cols)
{
    _type = QueryType::SELECT;
    _cols = std::move(cols);
    return this;
}

AbstractQueryBuilder* AbstractQueryBuilder::insert(std::vector<std::string> cols)
{
    _type = QueryType::INSERT;
    _cols = std::move(cols);
    return this;
}

AbstractQueryBuilder* AbstractQueryBuilder::update(std::vector<std::pair<std::string, std::string>> conds)
{
    _type = QueryType::UPDATE;
    _set = std::move(conds);
    return this;
}

AbstractQueryBuilder* AbstractQueryBuilder::update(std::pair<std::string, std::string> set)
{
    _type = QueryType::UPDATE;
    _set = {std::move(set)};
    return this;
}

AbstractQueryBuilder* AbstractQueryBuilder::where(std::vector<std::pair<std::string, std::string>> where)
{
    _where = std::move(where);
    return this;
}

AbstractQueryBuilder* AbstractQueryBuilder::where(std::pair<std::string, std::string> where)
{
    _where = {std::move(where)};
    return this;
}

AbstractQueryBuilder *AbstractQueryBuilder::opt_where(std::vector<std::pair<std::string, std::string>> where)
{
    _opt_where = std::move(where);
    return this;
}


AbstractQueryBuilder* AbstractQueryBuilder::values(std::vector<std::vector<std::string>> vals)
{
    if (_type == QueryType::INSERT)
        _inserts = std::move(vals);
    return this;
}

AbstractQueryBuilder* AbstractQueryBuilder::values(std::vector<std::vector<std::string>>* vals)
{
    if (_type == QueryType::INSERT)
        _inserts = *vals;
    return this;
}
