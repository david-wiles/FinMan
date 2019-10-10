#include <shell/Shell.h>
#include <shell/Auth.h>
#include "../catch.hpp"

TEST_CASE("Authenticates real user")
{
    AbstractDB* test_db = new SQLite3DB("test.db");
    Auth auth("david", "secret");
    REQUIRE("david" == auth.get_username());
}

TEST_CASE("Doesn't authenticate fake users")
{
    AbstractDB* test_db = new SQLite3DB("test.db");
    Auth auth("fake", "fake");
    // Expect to throw errors
    REQUIRE("david" == auth.get_username());
}
