#ifndef PIGGYBANK_ACCOUNT_HXX
#define PIGGYBANK_ACCOUNT_HXX

#include <string>
#include <vector>

#include <odb/core.hxx>
#include "transaction.hxx"
#include "auth_user.hxx"

#pragma db object
class account
{
public:
    account();

    ~account();

protected:
    friend class odb::access;

    #pragma db id
    long _acct_num;
    double _balance;
    float _interest;

    std::vector<lazy_shared_ptr<transaction>> _transactions;
    #pragma db not_null
    std::shared_ptr<auth_user> _owner;
    #pragma db not_null
    std::shared_ptr<auth_user> _custodian;
};

#pragma db object
class retirement_account : public account
{
public:
    retirement_account();

    ~retirement_account();

private:
    friend class odb::access;
};

#endif //PIGGYBANK_ACCOUNT_HXX
