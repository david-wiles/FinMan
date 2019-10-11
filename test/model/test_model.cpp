#include "db/SQLite3Instance.h"
#include "../catch.hpp"


TEST_CASE("Connects to database")
{
    AbstractDBInstance* builder = new SQLite3Instance("test.db");

    std::vector<std::string> cols({"name"});
    std::vector<std::vector<std::string>> rows({
        {"sqlite"},
    });
    QueryResult expected(&cols, &rows);
    QueryResult* actual = builder->query("SELECT name FROM sqlite_master;", nullptr);

    REQUIRE(expected.get_rows() == actual->get_rows());
}
