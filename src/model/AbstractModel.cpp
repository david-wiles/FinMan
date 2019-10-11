#include "model/AbstractModel.h"

#include <utility>
#include <db/SQLite3Instance.h>
#include <stdexcept>

void AbstractModel::update(std::vector<std::pair<std::string, std::string>> set)
{
    _builder->update(std::move(set));

    if (SQLite3Instance::getInstance()->query(_builder) == nullptr) {
        throw std::runtime_error("Object not updated.");
    }
}

void AbstractModel::del()
{
    _builder->del();
    if (SQLite3Instance::getInstance()->query(_builder) == nullptr) {
        throw std::runtime_error("Object not deleted");
    }
}
