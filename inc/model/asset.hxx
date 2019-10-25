#ifndef PIGGYBANK_ASSET_HXX
#define PIGGYBANK_ASSET_HXX

#include <string>

#include <odb/core.hxx>
#include "auth_user.hxx"


class asset
{
public:
    asset();

private:
    friend class odb::access;

    unsigned int _id;
//    shared_ptr<auth_user> _owner;
    double _value;
    std::string _name;
    std::string _type;

};

#pragma db object(asset);
#pragma db member(asset::_id) id

#endif //PIGGYBANK_ASSET_HXX
