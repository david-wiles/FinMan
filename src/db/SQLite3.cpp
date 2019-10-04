#include <uuid/uuid.h>
#include <unordered_map>

#include "db/SQLite3.h"


SQLite3::SQLite3()
{
    this->db_instance = nullptr;
    if (!sqlite3_open("data.db", &this->db_instance))
        throw std::exception();
}

SQLite3* SQLite3::getInstance()
{
    if (SQLite3::instance == nullptr) {
        SQLite3::instance = new SQLite3();
    }
    return SQLite3::instance;
}

// Callback for init db query. Main argument is a map of table names and a boolean value indicating if the table was
// seen in the result
//int check_tables(void* table_names, int argc, char ** argv, char ** azColName) {
//    std::unordered_map<std::string, bool>* tables = static_cast<std::unordered_map<std::string, bool> *>(table_names);
//    tables->at(std::string(argv[0])) = true;
//}
//
//void SQLite3::init_db()
//{
//    char sql[] = "SELECT name FROM sqlite_master WHERE type='table';";
//    this->err = nullptr;
//
//    std::unordered_map<std::string, bool>* tables = new std::unordered_map<std::string, bool>({
//        {"auth_user", false},
//        {"transaction", false}
//        // Add the rest of the tables
//    });
//
//    if (sqlite3_exec(this->db_instance, sql, check_tables, tables, &this->err) == SQLITE_OK) {
//        // Check if all tables were seen
//        for (const auto& itr: *tables) {
//
//        }
//    }
//}

std::vector<std::vector<std::string>>* SQLite3::execute(std::string sql, std::vector<std::string>* params)
{
    // Create a prepared statement from the sql string
    sqlite3_stmt* stmt = nullptr;
    int status = sqlite3_prepare_v2(this->db_instance, sql.c_str(), -1, &stmt, nullptr);

    // Bind parameters to prepared statement
    if (status == SQLITE_OK) {
        int i = 1;
        for (const auto &itr: *params) {
            const char* str = itr.c_str();
            sqlite3_bind_text(stmt, i++, str, sizeof(str), SQLITE_STATIC);
        }
    } else {
        // Throw error
    }

    int step = sqlite3_step(stmt);

    // Get column names
    auto* results = new std::vector<std::vector<std::string>>;
    int num_cols = sqlite3_column_count(stmt);
    auto* col_names = new std::vector<std::string>(num_cols);
    for (int i = 0; i < num_cols; ++i)
        col_names->emplace_back((char *) sqlite3_column_name(stmt, i));

    // If rows were returned, add it to the return array
    while (step != SQLITE_DONE) {
        auto* row = new std::vector<std::string>(num_cols);
        for (int i = 0; i < num_cols; ++i)
            row->emplace_back((char *)sqlite3_column_text(stmt, i));
        results->push_back(*row);
        step = sqlite3_step(stmt);
    }

    // Commit transaction and return results
    sqlite3_finalize(stmt);
    return results;
}

SQLite3::~SQLite3()
{
    sqlite3_close(this->db_instance);
    delete SQLite3::instance;
}
