#ifndef PIGGYBANK_BUDGET_HXX
#define PIGGYBANK_BUDGET_HXX

#include <string>

#include <odb/core.hxx>
#include "auth_user.hxx"

class budget
{
public:
    budget();

private:
    friend class odb::access;

    unsigned int _id;
//    shared_ptr<auth_user> _user;
//    shared_ptr<family> _family;
    double _amount;
    std::string description;
};

#pragma db object(budget)
#pragma db member(budget::_id) id

#endif //PIGGYBANK_BUDGET_HXX
