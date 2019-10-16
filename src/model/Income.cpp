#include <db/SQLite3Instance.h>
#include "model/Income.h"

bool Income::create(const std::vector<std::string> &vals)
{
    if (vals.size() != 5)
        return false;

    auto* query = new SQLite3QueryBuilder("income");
    query
    ->insert({"owner", "amount", "type", "hours", "pay_freq"})
    ->values({vals});

    return SQLite3Instance::getInstance()->query(query) != nullptr;
}
