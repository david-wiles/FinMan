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

    void select(std::vector<std::string> cols);
    void insert(std::vector<std::string> cols);
    void del() { this->_type = QueryType::DELETE; };
    void update(std::vector<std::pair<std::string,std::string>> set);
    void update(std::pair<std::string, std::string> cond);

    void where(std::vector<std::pair<std::string,std::string>> where);
    void where(std::pair<std::string, std::string> where);
    void values(std::vector<std::vector<std::string>> vals);

    ~AbstractQueryBuilder() { delete(_vals); };
protected:
    std::string _table;
    QueryType _type;
    std::vector<std::pair<std::string,std::string>> _where;
    std::vector<std::vector<std::string>> _inserts;
    std::vector<std::pair<std::string,std::string>> _set;
    std::vector<std::string> _cols;

    std::string _sql;
    std::vector<std::string>* _vals;
};


#endif //FINMAN_ABSTRACTQUERYBUILDER_H
