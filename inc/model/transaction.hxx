#ifndef PIGGYBANK_TRANSACTION_HXX
#define PIGGYBANK_TRANSACTION_HXX

#include <string>
#include <memory>

#include <odb/core.hxx>
#include "account.hxx"
#include "util.hxx"

#pragma db object abstract
class transaction
{
public:
    transaction();

    virtual ~transaction() = 0;

protected:
    friend class odb::access;

    #pragma db default(0.0)
    double _amount;
    #pragma db options("default current_timestamp")
    date _datetime;
    #pragam db default("")
    std::string _description;
};


#pragma db object
class withdrawal : public transaction
{
public:
    withdrawal();

    ~withdrawal();

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
    deposit();

    ~deposit();

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    std::shared_ptr<account> _to_acct;
};

#pragma db object
class transfer : public transaction
{
public:
    transfer();

    ~transfer();

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    std::shared_ptr<account> _to_acct;
    #pragma db not_null
    std::shared_ptr<account> _from_acct;
};


#endif //PIGGYBANK_TRANSACTION_HXX
