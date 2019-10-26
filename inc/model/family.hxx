#ifndef PIGGYBANK_FAMILY_HXX
#define PIGGYBANK_FAMILY_HXX

#include <string>

#include <odb/core.hxx>


#pragma db object
class family
{
public:
    family();

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    std::string _owner;

};


#endif //PIGGYBANK_FAMILY_HXX
