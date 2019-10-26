#ifndef PIGGYBANK_AUTH_USER_HXX
#define PIGGYBANK_AUTH_USER_HXX

#include <string>
#include <memory>

#include <odb/core.hxx>
#include "family.hxx"


#pragma db object
class auth_user
{
public:
    auth_user() = default;

    ~auth_user() = default;

private:
    friend class odb::access;

    #pragma db id
    std::string _username;
    #pragma db not_null
    std::string _hash;

    std::shared_ptr<family> _family;
};

#endif //PIGGYBANK_AUTH_USER_HXX
