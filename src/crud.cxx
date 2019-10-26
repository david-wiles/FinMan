#include "crud.hxx"

#include <memory>

#include <odb/sqlite/database.hxx>

template <class T>
int crud::create(T obj)
{
    std::unique_ptr<odb::database> db(new odb::sqlite::database("data.db", SQLITE_OPEN_READWRITE));
    odb::transaction t(db->begin());

    unsigned int id = db->persist(obj);

    t.commit();

    return id;
}

template <class T>
T crud::read()
{
    std::unique_ptr<odb::database> db(new odb::sqlite::database("data.db", SQLITE_OPEN_READWRITE));
    odb::transaction t(db->begin());


}

template <class T>
bool crud::update(T obj)
{

}

template <class T>
bool crud::remove(T obj)
{

}
