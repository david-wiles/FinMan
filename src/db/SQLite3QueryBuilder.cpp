#include <stdexcept>
#include "db/SQLite3QueryBuilder.h"

std::string SQLite3QueryBuilder::build_where(int val_index)
{

    if (_opt_where.empty() && _where.empty())
        throw std::runtime_error("Where condition empty");

    std::string where_str;

    // Build or conditions
    if (!_opt_where.empty()) {

        where_str += " ( ";

        int opt_size = _opt_where.size();
        for (int i = 0; i < opt_size - 1; ++i) {
            std::pair<std::string, std::string> pair = _where.at(i);
            where_str += pair.first + " = ?" + std::to_string(val_index++) + " OR ";
            _vals->push_back(pair.second);
        }
        where_str += _where.at(opt_size - 1).first + " = ?" + std::to_string(val_index++) + " ) ";
        _vals->push_back(_where.at(opt_size - 1).second);

        if (!_where.empty())
            where_str += " AND ";
    }

    // Build and conditions
    if (!_where.empty()) {
        int where_size = _where.size();
        for (int i = 0; i < where_size - 1; ++i) {
            std::pair<std::string, std::string> pair = _where.at(i);
            where_str += pair.first + " = ?" + std::to_string(val_index++) + " AND ";
            _vals->push_back(pair.second);
        }
        where_str += _where.at(where_size - 1).first + " = ?" + std::to_string(val_index);
        _vals->push_back(_where.at(where_size - 1).second);
    }

    return where_str;
}

void SQLite3QueryBuilder::build()
{
    // Clear any previous build
    _sql = "";
    _vals = new std::vector<std::string>();

    // Build insert
    if (_type == QueryType::INSERT) {

        _sql += "INSERT INTO " + _table + " ( ";

        // Insert columns
        if (_cols.empty()) {
            throw std::runtime_error("Columns must be specified.");
        } else {
            int num_cols = _cols.size();
            for (int i = 0; i < num_cols - 1; ++i)
                _sql += _cols.at(i) + ", ";
            _sql += _cols.at(num_cols - 1) + " ) VALUES ";
        }

        // Add new rows
        int index = 1;
        for (auto &itr: _inserts) {
            std::string row("( ");
            int len = itr.size();
            for (int i = 0; i < len - 1; ++i) {
                row += "?" + std::to_string(index++) + ", ";
                _vals->push_back(itr.at(i));
            }
            row += "?" + std::to_string(index++) + " ), ";
            _vals->push_back(itr.at(len-1));

            _sql += row;
        }

        _sql += ";";

    // Build select
    } else if (_type == QueryType::SELECT) {

        _sql += "SELECT ";

        // Returning columns
        if (_cols.empty()) {
            _sql += "* ";
        } else {
            int num_cols = _cols.size();
            for (int i = 0; i < num_cols - 1; ++i)
                _sql += _cols.at(i) + ", ";
            _sql += _cols.at(num_cols - 1);
        }

        _sql += " FROM " + _table + " WHERE " + build_where(1) + ";";

    // Build update
    } else if (_type == QueryType::UPDATE) {
        
        _sql += "UPDATE " + _table + " SET ";

        // Build set statement
        int index = 1;
        int set_size = _set.size();
        for (int i = 0; i < set_size - 1; ++i) {
            std::pair<std::string, std::string> pair = _set.at(i);
            _sql += pair.first + " = ?" + std::to_string(index++) + ", ";
            _vals->push_back(pair.second);
        }
        _sql += _set.at(set_size - 1).first + " = ?" + std::to_string(index++);
        _vals->push_back(_set.at(set_size - 1).second);

        _sql += " WHERE " + build_where(index) + ";";

    // Build delete
    } else if (_type == QueryType::DELETE) {

        _sql += "DELETE FROM " + _table + " WHERE " + build_where(1);

    }

}
