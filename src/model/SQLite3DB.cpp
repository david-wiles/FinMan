#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include "model/SQLite3DB.h"


AbstractDB* AbstractDB::_instance = nullptr;

SQLite3DB::SQLite3DB()
{
    this->db_instance = nullptr;
    this->err = nullptr;

    if (sqlite3_open("assets/data.db", &this->db_instance) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(this->db_instance));
}

SQLite3DB::SQLite3DB(std::string db_file)
{
    this->db_instance = nullptr;
    this->err = nullptr;

    if (sqlite3_open(db_file.c_str(), &this->db_instance) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(this->db_instance));
}

AbstractDB* SQLite3DB::getInstance()
{
    // Checking if the db_instance pointer is null should have the effect of checking if the instance has been set
    if (AbstractDB::_instance == nullptr) {
        AbstractDB::_instance = new SQLite3DB();
    }
    return AbstractDB::_instance;
}

// Callback for init db query, sets tables seen to true in tables map
int check_tables(void* table_names, int argc, char ** argv, char ** azColName) {
    auto* tables = static_cast<std::unordered_map<std::string, bool> *>(table_names);
    if (argv[0] != nullptr) {
        try {
            tables->at(std::string(argv[0])) = true;
        } catch (std::out_of_range& ex) {
            // DEBUG
            std::cerr << "Unknown table encountered." << ex.what() << std::endl;
        }
    }
    return 0;
}

void SQLite3DB::init_db()
{
    char sql[] = "SELECT name FROM sqlite_master WHERE type IN ('table','view') AND name NOT LIKE 'sqlite_%'";
    this->err = nullptr;

    auto* tables = new std::unordered_map<std::string, bool>({
        {"auth_user", false},
        // Add the rest of the tables
    });

    if (sqlite3_exec(this->db_instance, sql, check_tables, tables, &this->err) == SQLITE_OK) {
        bool initialized = true;

        // TODO how to check if only one (or more) tables need to be initialized, as opposed to recreating the db
        // Check if all tables were seen
        for (const auto& itr: *tables) {
            if (itr.second == false)
                initialized = false;
        }

        delete(tables);

        if (!initialized) {
            // Get SQL from file
            std::ifstream init_file("assets/db.sql");
            std::string init_sql((std::istreambuf_iterator<char>(init_file)), (std::istreambuf_iterator<char>()));

            // Execute initialization sql
            this->err = nullptr;
            if (sqlite3_exec(this->db_instance, init_sql.c_str(), nullptr, nullptr, &this->err) != SQLITE_OK)
                throw std::runtime_error(sqlite3_errmsg(this->db_instance));
        }
    } else
        throw std::runtime_error(sqlite3_errmsg(this->db_instance));
}

QueryResult* SQLite3DB::query(std::string sql, std::vector<std::string>* params)
{
    sqlite3_stmt* stmt = nullptr;
    this->err = nullptr;
    int param = 1;

    // Create a prepared statement from the sql string
    if (sqlite3_prepare_v2(this->db_instance, (const char*) sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind parameters to prepared statement
        for (const auto &itr: *params)
            sqlite3_bind_text(stmt, param++, itr.c_str(), itr.size(), SQLITE_STATIC);

    } else
        throw std::runtime_error(sqlite3_errmsg(this->db_instance));

    int step = sqlite3_step(stmt);

    // Get column names
    int num_cols = sqlite3_column_count(stmt);
    auto* col_names = new std::vector<std::string>(num_cols);
    for (int i = 0; i < num_cols; ++i)
        col_names->at(i) = std::string((char *) sqlite3_column_name(stmt, i));

    // Add each row returned into results vector
    auto* results = new std::vector<std::vector<std::string>>;
    while (step != SQLITE_DONE) {
        auto* row = new std::vector<std::string>(num_cols);
        for (int i = 0; i < num_cols; ++i) {
            char* col_text = (char *) sqlite3_column_text(stmt, i);
            if (col_text != nullptr)
                row->at(i) = std::string(col_text);
        }
        results->push_back(*row);
        step = sqlite3_step(stmt);
    }

    // Commit transaction and return results
    sqlite3_finalize(stmt);
    return new QueryResult(col_names, results);
}

SQLite3DB::~SQLite3DB()
{
    sqlite3_close(this->db_instance);
}
