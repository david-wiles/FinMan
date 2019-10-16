#include "model/AbstractModel.h"

#include <utility>
#include <db/SQLite3Instance.h>
#include <stdexcept>
#include <view/TableView.h>


QueryResult* AbstractModel::get()
{
    // Make sure that query returns all columns
    _builder->select({});

    auto* res = SQLite3Instance::getInstance()->query(_builder);

    if (res->row_count() != 1) {
        return nullptr;
    } else {
        return res;
    }
}


bool AbstractModel::update(std::vector<std::pair<std::string, std::string>> set)
{
    _builder->update(std::move(set));
    return SQLite3Instance::getInstance()->query(_builder) != nullptr;
}

void AbstractModel::del()
{
    _builder->del();
    if (SQLite3Instance::getInstance()->query(_builder) == nullptr) {
        throw std::runtime_error("Object not deleted");
    }
}

AbstractView *AbstractModel::get_view()
{
    auto obj = this->get();

    if (obj != nullptr)
        return new TableView(obj);
    else
        return nullptr;
}
