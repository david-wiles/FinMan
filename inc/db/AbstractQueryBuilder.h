#ifndef FINMAN_ABSTRACTQUERYBUILDER_H
#define FINMAN_ABSTRACTQUERYBUILDER_H


#include <string>
#include <utility>
#include <vector>

// Enum of supported SQL queries. Other types of SQL statements should be written by hand
enum class QueryType {SELECT, INSERT, DELETE, UPDATE};

/**
 * Abstract QueryBuilder class. Provides an API for creating SQL statements independent of dialect.
 *
 * The basic usage should be to specify which table to query with the constructor, with further options specified with
 * methods.  The data passed to each method is stored in the class instance, and calling build() will write a SQL
 * statement in the desired dialect (in a subclass)
 */
class AbstractQueryBuilder
{
public:
    // Constructor specifies table
    explicit AbstractQueryBuilder(std::string table)
    : _table(std::move(table)), _type(), _vals() {};

    virtual void build() = 0;
    std::string get_sql() { return this->_sql; };
    std::vector<std::string>* get_params() { return this->_vals; };

    // Columns to return
    AbstractQueryBuilder* select(std::vector<std::string> cols);

    // Columns to insert into
    AbstractQueryBuilder* insert(std::vector<std::string> cols);

    AbstractQueryBuilder* del() { this->_type = QueryType::DELETE; return this; };

    // Values to set (existing rows)
    AbstractQueryBuilder* update(std::vector<std::pair<std::string,std::string>> set);
    AbstractQueryBuilder* update(std::pair<std::string, std::string> set);

    // WHERE conditions separated by AND
    AbstractQueryBuilder* where(std::vector<std::pair<std::string,std::string>> where);
    AbstractQueryBuilder* where(std::pair<std::string, std::string> where);
    // WHERE statements separated by OR
    AbstractQueryBuilder* opt_where(std::vector<std::pair<std::string,std::string>> where);

    // Values to insert (new rows)
    AbstractQueryBuilder* values(std::vector<std::vector<std::string>> vals);
    AbstractQueryBuilder* values(std::vector<std::vector<std::string>>* vals);

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
