#ifndef PIGGYBANK_ACCOUNT_HXX
#define PIGGYBANK_ACCOUNT_HXX

#include <string>
#include <vector>

#include <odb/core.hxx>
#include "auth_user.hxx"


#pragma db object
class account
{
public:
    account() = default;
    account(double balance, float interest);

    ~account() = default;

protected:
    friend class odb::access;

    #pragma db id
    long _acct_num;
    double _balance;
    float _interest;

    #pragma db not_null
    std::shared_ptr<auth_user> _owner;
    #pragma db not_null
    std::shared_ptr<auth_user> _custodian;
};

#pragma db object
class retirement_account : public account
{
public:
    retirement_account() = default;

    ~retirement_account() = default;

private:
    friend class odb::access;
};

#endif //PIGGYBANK_ACCOUNT_HXX
