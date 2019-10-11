#ifndef TESTS_QUERYRESULT_H
#define TESTS_QUERYRESULT_H


#include <vector>
#include <string>

/**
 * Class containing a table of data returned from a query.
 *
 * The _col_names vector contains column names returned and _rows contains a vector of rows returned. Each row is
 * another vector, and columns are organized in the same order that they are specified in _col_names.
 */
class QueryResult
{
public:
    QueryResult(std::vector<std::string>* columns, std::vector<std::vector<std::string>>* rows) :
        _col_names(columns), _rows(rows) {};

    inline const std::string* get_column_name(int loc) const { return &_col_names->at(loc); };
    inline const std::vector<std::string>* get_column_names() const { return _col_names; };
    inline const std::vector<std::string>* get_row(int loc) const { return &_rows->at(loc); };
    inline const std::vector<std::vector<std::string>>* get_rows() const { return _rows; };
    inline int row_count() { return _rows->size(); };

    ~QueryResult() { delete(_col_names); delete(_rows); };

private:
    const std::vector<std::string>* _col_names;
    const std::vector<std::vector<std::string>>* _rows;
};


#endif //TESTS_QUERYRESULT_H
