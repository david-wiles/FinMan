#ifndef PIGGYBANK_INCOME_HXX
#define PIGGYBANK_INCOME_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"


#pragma db object abstract
class income
{
public:
    income() = default;

    virtual ~income() = 0;

protected:
    friend class odb::access;

    double _amount;
    int _pay_frequency;

    #pragma db not_null
    std::shared_ptr<account> _to_acct;

};

#pragma db object
class salaried_income : public income
{
public:
    salaried_income() = default;

    ~salaried_income() override = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;
};

#pragma db object
class hourly_income : public income
{
public:
    hourly_income() = default;

    ~hourly_income() override = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;
    #pragma db default(0.0)
    float _hours;
};

#endif //PIGGYBANK_INCOME_HXX
