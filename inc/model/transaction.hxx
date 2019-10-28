#ifndef PIGGYBANK_TRANSACTION_HXX
#define PIGGYBANK_TRANSACTION_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"


#pragma db object abstract
class transaction
{
public:
    transaction() = default;

    virtual ~transaction() = 0;

protected:
    friend class odb::access;

    #pragma db default(0.0)
    double _amount;
    #pragma db options("default current_timestamp")
    time_t _datetime;
    #pragma db default("")
    std::string _description;
    #pragma db default(0.0)
    double _new_acct_balance;
};

// Transaction types

#pragma db object
class withdrawal : public transaction
{
public:
    withdrawal() = default;

    ~withdrawal() override = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    std::shared_ptr<account> _from_acct;
};

#pragma db object
class deposit : public transaction
{
public:
    deposit() = default;

    ~deposit() override = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    std::shared_ptr<account> _to_acct;
};

#endif //PIGGYBANK_TRANSACTION_HXX
