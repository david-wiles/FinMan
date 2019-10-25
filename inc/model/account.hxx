#ifndef PIGGYBANK_ACCOUNT_HXX
#define PIGGYBANK_ACCOUNT_HXX

#include <string>

#include <odb/core.hxx>


class account
{
public:
    account();

private:
    friend class odb::access;

    long _acct_num;
    std::string _owner;
    std::string _custodian;
    double _balance;
    std::string _type;
    float _interest;
};

#pragma db object(account)
#pragma db member(account::_acct_num) id


#endif //PIGGYBANK_ACCOUNT_HXX
