#ifndef TESTS_DB_RESULT_H
#define TESTS_DB_RESULT_H


#include <vector>
#include <string>

class DB_Result
{
public:
    DB_Result(std::vector<std::string>* columns, std::vector<std::vector<std::string>>* rows) :
        _col_names(columns), _rows(rows) {};

    inline const std::string* get_column_name(int loc) const { return &_col_names->at(loc); };
    inline const std::vector<std::string>* get_column_names() const { return _col_names; };
    inline const std::vector<std::string>* get_row(int loc) const { return &_rows->at(loc); };
    inline const std::vector<std::vector<std::string>>* get_rows() const { return _rows; };
    inline int row_count() { return _rows->size(); };

    ~DB_Result() { delete(_col_names); delete(_rows); };

private:
    const std::vector<std::string>* _col_names;
    const std::vector<std::vector<std::string>>* _rows;
};


#endif //TESTS_DB_RESULT_H
