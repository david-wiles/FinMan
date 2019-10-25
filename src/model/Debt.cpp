#include <db/SQLite3QueryBuilder.h>
#include <db/SQLite3Instance.h>
#include "model/Debt.h"

bool Debt::create(const std::vector<std::string> &vals)
{
    if (vals.size() != 7)
        return false;

    auto query = new SQLite3QueryBuilder("debt");
    query->insert({"owner", "principal", "interest", "start_date", "maturity_date", "type", "from_acct"})->values({vals});

    bool res = SQLite3Instance::getInstance()->query(query) != nullptr;
    delete query;
    return res;
}
