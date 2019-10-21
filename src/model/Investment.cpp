#include <db/SQLite3QueryBuilder.h>
#include <db/SQLite3Instance.h>
#include "model/Investment.h"

bool Investment::create(const std::vector<std::string> &vals)
{
    if (vals.size() != 7)
        return false;
    //{username, type, ticker, buy_price, num_shares, buy_date, acct}

    auto query = new SQLite3QueryBuilder("investment");
    query->insert({"owner", "type", "ticker", "buy_price", "num_shares", "buy_date", "acct"})->values({vals});

    return SQLite3Instance::getInstance()->query(query) != nullptr;

}
