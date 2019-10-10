#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <fstream>

#include "model/SQLite3QueryBuilder.h"


AbstractQueryBuilder* AbstractQueryBuilder::_instance = nullptr;

SQLite3QueryBuilder::SQLite3QueryBuilder()
{
    this->db_instance = nullptr;
    this->err = nullptr;

    if (sqlite3_open("assets/data.db", &this->db_instance) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(this->db_instance));
}

SQLite3QueryBuilder::SQLite3QueryBuilder(std::string db_file)
{
    this->db_instance = nullptr;
    this->err = nullptr;

    if (sqlite3_open(db_file.c_str(), &this->db_instance) != SQLITE_OK)
        throw std::runtime_error(sqlite3_errmsg(this->db_instance));
}

AbstractQueryBuilder* SQLite3QueryBuilder::getInstance()
{
    // Checking if the db_instance pointer is null should have the effect of checking if the instance has been set
    if (AbstractQueryBuilder::_instance == nullptr) {
        AbstractQueryBuilder::_instance = new SQLite3QueryBuilder();
    }
    return AbstractQueryBuilder::_instance;
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

void SQLite3QueryBuilder::init_db()
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

bool SQLite3QueryBuilder::createRow(std::string table, std::vector<std::pair<std::string, std::string>> *kv_insert)
{
    std::string insert_cols;
    std::string inserts;
    auto* vals = new std::vector<std::string>();

    int insert_size = kv_insert->size();
    for (int i = 0; i < insert_size - 2; ++i) {
        std::pair<std::string,std::string> pair = kv_insert->at(i);
        insert_cols += pair.first + ", ";
        inserts += "?" + std::to_string(i + 1) + ", ";
        vals->push_back(pair.second);
    }
    insert_cols += kv_insert->at(insert_size - 1).first;
    inserts += "?" + std::to_string(insert_size);

    std::string sql = "INSERT INTO " + table + " ( " + insert_cols + " ) VALUES ( " + inserts + " );";

    try {
        this->query(sql, vals);
        return true;
    } catch (std::runtime_error &err) {
        std::cout << "Could not create records: " << err.what() << std::endl;
        return false;
    }
}

QueryResult *SQLite3QueryBuilder::findRow(std::string table, std::vector<std::string> *ret_cols,
                                          std::vector<std::pair<std::string, std::string>> *kv_where)
{
    std::string select_cols;
    std::string where;
    auto* vals = new std::vector<std::string>();

    if (ret_cols == nullptr) {
        select_cols = " * ";
    } else if (ret_cols->empty()) {
        select_cols = " * ";
    } else {
        int ret_cols_size = ret_cols->size();
        for (int i = 0; i < ret_cols_size - 2; ++i)
            select_cols += ret_cols->at(i) + ", ";
        select_cols += ret_cols->at(ret_cols_size - 1);
    }

    int where_size = kv_where->size();
    for (int i = 0; i < where_size - 2; ++i) {
        std::pair<std::string,std::string> pair(kv_where->at(i));
        where += pair.first + " = " + "?" + std::to_string(i+1) + " AND ";
        vals->push_back(pair.second);
    }
    where += kv_where->at(where_size - 1).first + " = " + "?" + std::to_string(where_size);

    std::string sql = "SELECT " + select_cols + "FROM " + table + " WHERE " + where + ";";

    QueryResult* result = this->query(sql, vals);

    delete(vals);
    return result;
}

bool SQLite3QueryBuilder::updateRow(std::string table, std::vector<std::pair<std::string, std::string>> *kv_update,
                                    std::vector<std::pair<std::string, std::string>> *kv_where)
{
    return false;
}

bool SQLite3QueryBuilder::deleteRow(std::string table, std::vector<std::pair<std::string, std::string>> *kv_where)
{
    return false;
}


QueryResult* SQLite3QueryBuilder::query(std::string sql, std::vector<std::string>* params)
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

SQLite3QueryBuilder::~SQLite3QueryBuilder()
{
    sqlite3_close(this->db_instance);
}
