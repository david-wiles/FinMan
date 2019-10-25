#ifndef PIGGYBANK_AUTH_USER_HXX
#define PIGGYBANK_AUTH_USER_HXX

#include <string>
#include <memory>


#include <odb/core.hxx>
#include "family.hxx"


class auth_user
{
public:
    auth_user();

private:
    friend class odb::access;

    std::string _username;
    std::string _hash;
//    std::shared_ptr<family> _family;
};

#pragma db object(auth_user)
#pragma db member(auth_user::_username) id

#endif //PIGGYBANK_AUTH_USER_HXX
