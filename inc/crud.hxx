#ifndef PIGGYBANK_MGMT_HXX
#define PIGGYBANK_MGMT_HXX

/**
 * Shortcuts to shorten code for single CRUD operations
 */
namespace crud
{

    template <class T>
    int create(T obj);

    template <class T>
    T crud::read();

    template <class T>
    bool update(T obj);

    template <class T>
    bool remove(T obj);

}


#endif //PIGGYBANK_MGMT_HXX
