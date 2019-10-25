#ifndef PIGGYBANK_DEBT_HXX
#define PIGGYBANK_DEBT_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"

class debt
{
public:
    debt();

private:
    friend class odb::access;

    unsigned int _id;
    double _principal;
    float _interest;
    time_t _start_date;
    time_t _end_date;
    std::string _type;
//    shared_ptr<account> _from_acct;

};

#pragma db object(debt)
#pragma db member(debt::_id) id

#endif //PIGGYBANK_DEBT_HXX
