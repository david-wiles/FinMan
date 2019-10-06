#ifndef TESTS_DB_RESULT_H
#define TESTS_DB_RESULT_H


#include <vector>
#include <string>

class DB_Result
{
public:
    DB_Result(std::vector<std::string>* columns, std::vector<std::vector<std::string>>* rows) :
        _col_names(columns), _rows(rows) {};

    const std::string* get_column_name(int loc) const;
    const std::vector<std::string>* get_column_names() const;
    const std::vector<std::string>* get_row(int loc) const;
    const std::vector<std::vector<std::string>>* get_rows() const;
    int row_count();

    ~DB_Result() = default;

private:
    const std::vector<std::string>* _col_names;
    const std::vector<std::vector<std::string>>* _rows;
};


#endif //TESTS_DB_RESULT_H
