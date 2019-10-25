#include <db/SQLite3Instance.h>
#include "model/Income.h"

bool Income::create(const std::vector<std::string> &vals)
{
    if (vals.size() != 6)
        return false;

    auto query = new SQLite3QueryBuilder("income");
    query
    ->insert({"owner", "type", "hours", "amount", "pay_frequency", "to_acct"})
    ->values({vals});

    bool res = SQLite3Instance::getInstance()->query(query) != nullptr;
    delete query;
    return res;
}
