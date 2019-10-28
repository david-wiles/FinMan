#ifndef PIGGYBANK_ASSET_HXX
#define PIGGYBANK_ASSET_HXX

#include <string>
#include <memory>

#include <odb/core.hxx>
#include "auth_user.hxx"
#include "account.hxx"


#pragma db object
class asset
{
public:
    asset() = default;
    asset(double value, std::string name, std::string type);

    ~asset() = default;
private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;
    double _value;
    std::string _name;
    std::string _type;

    std::shared_ptr<auth_user> _owner;
};

#endif //PIGGYBANK_ASSET_HXX
