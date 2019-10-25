#ifndef PIGGYBANK_FAMILY_HXX
#define PIGGYBANK_FAMILY_HXX

#include <string>

#include <odb/core.hxx>


class family
{
public:
    family();

private:
    friend class odb::access;

    unsigned int _id;
    std::string _owner;

};

#pragma db object(family)
#pragma db member(family::_id) id

#endif //PIGGYBANK_FAMILY_HXX
