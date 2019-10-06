#include "db/DB_Result.h"


const std::string* DB_Result::get_column_name(int loc) const
{
    return &_col_names->at(loc);
}

const std::vector<std::string>* DB_Result::get_column_names() const
{
    return _col_names;
}

const std::vector<std::string>* DB_Result::get_row(int loc) const
{
    return &_rows->at(loc);
}

const std::vector<std::vector<std::string>>* DB_Result::get_rows() const
{
    return _rows;
}

int DB_Result::row_count()
{
    return _rows->size();
}