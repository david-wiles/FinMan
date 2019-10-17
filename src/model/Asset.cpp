#include <db/SQLite3QueryBuilder.h>
#include <db/SQLite3Instance.h>
#include "model/Asset.h"

bool Asset::create(const std::vector<std::string> &vals)
{
    if (vals.size() != 4)
        return false;

    // TODO input validation

    auto query = new SQLite3QueryBuilder("asset");
    query
    ->insert({"owner", "value", "name", "type", "buy_date"})
    ->values({vals});

    return SQLite3Instance::getInstance()->query(query) != nullptr;
}
