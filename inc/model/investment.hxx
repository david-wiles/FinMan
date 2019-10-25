#ifndef PIGGYBANK_INVESTMENT_HXX
#define PIGGYBANK_INVESTMENT_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"
#include "auth_user.hxx"

class investment
{
public:
    investment();

private:
    friend class odb::access;

    unsigned int _id;
    std::string _type; // TODO make abstract parent investment w/ type subclasses
    std::string _ticker;
    double _buy_price;
    double _num_shares;
    time_t buy_date;
//    shared_ptr<account> _acct;
//    shared_ptr<auth_user> _owner;

};

#pragma db object(investment)
#pragma db member(investment::_id) id

#endif //PIGGYBANK_INVESTMENT_HXX
