#include "model/SQLite3DB.h"
#include "../catch.hpp"


TEST_CASE("Connects to database")
{
    AbstractDB* builder = new SQLite3DB("test.db");

    std::vector<std::string> cols({"name"});
    std::vector<std::vector<std::string>> rows({
        {"sqlite"},
    });
    QueryResult expected(&cols, &rows);
    QueryResult* actual = builder->query("SELECT name FROM sqlite_master;", nullptr);

    REQUIRE(expected.get_rows() == actual->get_rows());
}
