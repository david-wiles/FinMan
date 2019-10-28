#ifndef PIGGYBANK_DEBT_HXX
#define PIGGYBANK_DEBT_HXX

#include <string>

#include <odb/core.hxx>
#include "account.hxx"


#pragma db object abstract
class debt
{
public:
    debt() = default;
    debt(double principal, float interest, time_t start_date, time_t end_date);

    virtual ~debt() = 0;

protected:
    friend class odb::access;

    double _principal;
    float _interest;
    time_t _start_date;
    time_t _end_date;

    #pragma db not_null
    std::shared_ptr<account> _from_acct;

};

#pragma db object
class mortgage : public debt
{
public:
    mortgage() = default;

    ~mortgage() = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;
};

#endif //PIGGYBANK_DEBT_HXX
