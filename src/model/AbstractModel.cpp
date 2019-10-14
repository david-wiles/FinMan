#include "model/AbstractModel.h"

#include <utility>
#include <db/SQLite3Instance.h>
#include <stdexcept>

AbstractModel::AbstractModel(AbstractQueryBuilder *builder)
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

std::string AbstractModel::get_attr(const std::string &attr_name)
{
    // There's definitely a better way to do this
    int index = 0;
    for (auto& itr: *_obj->get_column_names()) {
        if (itr == attr_name)
            return _obj->get_row(1)->at(index);
        ++index;
    }

    return std::string();
}
