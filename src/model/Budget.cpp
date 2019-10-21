#include <db/SQLite3QueryBuilder.h>
#include <db/SQLite3Instance.h>
#include "model/Budget.h"

bool Budget::create(const std::vector<std::string> &vals)
{
    if (vals.size() != 4)
        return false;

    auto query = new SQLite3QueryBuilder("budget");
    query->insert({"user", "goal_acct", "goal_amount", "description"})->values({vals});

    return SQLite3Instance::getInstance()->query(query) != nullptr;
}
