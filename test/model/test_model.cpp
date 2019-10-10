#include "../inc/model/SQLite3QueryBuilder.h"
#include "../catch.hpp"


TEST_CASE("Connects to database")
{
    AbstractQueryBuilder* builder = new SQLite3QueryBuilder("test.db");

    std::vector<std::string> cols({"name"});
    std::vector<std::vector<std::string>> rows({
        {"sqlite"},
    });
    QueryResult expected(&cols, &rows);
    QueryResult* actual = builder->execute("SELECT name FROM sqlite_master;", nullptr);

    REQUIRE(expected.get_rows() == actual->get_rows());
}
