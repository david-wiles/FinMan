#ifndef PIGGYBANK_INCOME_HXX
#define PIGGYBANK_INCOME_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"

class income
{
public:
    income();

private:
    friend class odb::access;

    unsigned int _id;
    float _hours;
    double _amount;
    int _pay_frequency;
//    shared_ptr<account> _to_acct;
    std::string type; // TODO make abstract parent class income with subclass salaray & hourly

};

#pragma db object(income)
#pragma db member(income::_id) id

#endif //PIGGYBANK_INCOME_HXX
