#ifndef PIGGYBANK_TRANSACTION_HXX
#define PIGGYBANK_TRANSACTION_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"

class transaction
{
public:
    transaction();

private:
    friend class odb::access;

    unsigned int _id;
    std::string _type; // TODO make abstract parent transaction with subclasses
    double _amount;
//    shared_ptr<account> _to_acct;
//    shared_ptr<account> _from_acct;
    time_t _datetime;
    std::string _description;
};

#pragma db object(transaction)
#pragma db member(transaction::_id) id

#endif //PIGGYBANK_TRANSACTION_HXX
