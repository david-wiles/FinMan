#include <db/SQLite3Instance.h>
#include <stdexcept>
#include <view/AccountView.h>
#include "model/Account.h"

Account::Account(AbstractQueryBuilder *builder) : AbstractModel(builder)
{
    // Make sure that query returns all columns
    builder->select({});

    auto* res = SQLite3Instance::getInstance()->query(builder);

    if (res->row_count() != 1) {
        throw std::runtime_error("WHERE condition is ambiguous, should only return one row.");
    } else {
        _obj = res;
        _builder = builder;
    }
}

AbstractView *Account::get_view()
{
    return new AccountView(this->_obj);
}

