#include "model/asset.hxx"

#include <utility>

asset::asset(double value, std::string name, std::string type) :
    _value(value), _name(std::move(name)), _type(std::move(type))
{
    // Set owner from user currently logged in
}
