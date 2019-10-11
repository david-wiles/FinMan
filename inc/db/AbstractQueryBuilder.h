#ifndef FINMAN_ABSTRACTQUERYBUILDER_H
#define FINMAN_ABSTRACTQUERYBUILDER_H


#include <string>
#include <utility>
#include <vector>

enum class QueryType {SELECT, INSERT, DELETE, UPDATE};

class AbstractQueryBuilder
{
public:
    explicit AbstractQueryBuilder(std::string table)
    : _table(std::move(table)), _type(), _vals() {};

    virtual void build() = 0;
    std::string get_sql() { return this->_sql; };
    std::vector<std::string>* get_params() { return this->_vals; };

    AbstractQueryBuilder* select(std::vector<std::string> cols);
    AbstractQueryBuilder* insert(std::vector<std::string> cols);
    AbstractQueryBuilder* del() { this->_type = QueryType::DELETE; return this; };
    AbstractQueryBuilder* update(std::vector<std::pair<std::string,std::string>> set);
    AbstractQueryBuilder* update(std::pair<std::string, std::string> set);

    AbstractQueryBuilder* where(std::vector<std::pair<std::string,std::string>> where);
    AbstractQueryBuilder* where(std::pair<std::string, std::string> where);
    AbstractQueryBuilder* opt_where(std::vector<std::pair<std::string,std::string>> where);
    AbstractQueryBuilder* values(std::vector<std::vector<std::string>> vals);

    virtual ~AbstractQueryBuilder() = default;
protected:
    std::string _table;
    QueryType _type;

    std::vector<std::pair<std::string,std::string>> _where;
    std::vector<std::pair<std::string,std::string>> _opt_where;
    std::vector<std::vector<std::string>> _inserts;
    std::vector<std::pair<std::string,std::string>> _set;
    std::vector<std::string> _cols;

    std::string _sql;
    std::vector<std::string>* _vals;
};


#endif //FINMAN_ABSTRACTQUERYBUILDER_H
