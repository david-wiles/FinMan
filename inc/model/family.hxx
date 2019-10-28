#ifndef PIGGYBANK_FAMILY_HXX
#define PIGGYBANK_FAMILY_HXX

#include <string>

#include <odb/core.hxx>


#pragma db object
class family
{
public:
    family() = default;
    family(std::string owner);

    ~family() = default;

private:
    friend class odb::access;

    #pragma db id
    unsigned int _id;

    #pragma db not_null
    std::string _owner;

};


#endif //PIGGYBANK_FAMILY_HXX
