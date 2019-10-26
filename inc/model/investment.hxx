#ifndef PIGGYBANK_INVESTMENT_HXX
#define PIGGYBANK_INVESTMENT_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"
#include "auth_user.hxx"


#pragma db object abstract
class investment
{
public:
    investment() = default;

    virtual ~investment() = 0;
protected:
    friend class odb::access;

    #pragma db options("default current_timestamp")
    time_t _buy_date;

    std::shared_ptr<account> _acct;
    #pragma db not_null
    std::shared_ptr<auth_user> _owner;

};

#pragma db object
class stock : public investment
{
public:
    stock() = default;

    ~stock() override = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    std::string _ticker;
    #pragma db not_null
    double _buy_price;
    #pragma db not_null
    double _num_shares;
    #pragma db default(0.0)
    float _dividend;
};

#pragma db object
class bond : public investment
{
public:
    bond();

    ~bond();

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    double _amount;
    #pragma db not_null
    float _interest;
    #pragma db not_null
    time_t _maturity_date;
};

#endif //PIGGYBANK_INVESTMENT_HXX
