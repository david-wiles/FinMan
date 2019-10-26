#ifndef PIGGYBANK_BUDGET_HXX
#define PIGGYBANK_BUDGET_HXX

#include <string>

#include <odb/core.hxx>
#include <odb/lazy-ptr.hxx>
#include "auth_user.hxx"


#pragma db object
class budget
{
public:
    budget() = default;

    ~budget() = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;
    double _amount;
    std::string description;

    std::shared_ptr<auth_user> _user;
    odb::lazy_shared_ptr<family> _family;
};


#endif //PIGGYBANK_BUDGET_HXX
