#include <controller/Controller.h>
#include <db/SQLite3Instance.h>
#include "../catch.hpp"

TEST_CASE("Test account view")
{
    auto instance = new SQLite3Instance("test.db");
    instance->init_db();

    Controller::account("tester", {})
}
