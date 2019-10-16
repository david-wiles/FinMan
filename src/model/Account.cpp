#include <db/SQLite3Instance.h>
#include <stdexcept>
#include <db/SQLite3QueryBuilder.h>
#include <view/TableView.h>
#include "model/Account.h"


bool Account::create(const std::vector<std::string>& vals)
{
    if (vals.size() != 6)
        return false;

    auto* query = new SQLite3QueryBuilder("account");
    query
    ->insert({"acct_num", "owner", "custodian", "balance", "type", "interest"})
    ->values({vals});

    return SQLite3Instance::getInstance()->query(query) != nullptr;
}

void Account::del()
{
    // Make account inactive to preserve relational integrity
    this->_builder->update(std::make_pair("active", "false"));
    SQLite3Instance::getInstance()->query(this->_builder);
}
