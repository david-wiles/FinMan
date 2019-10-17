#ifndef FINMAN_ABSTRACTMODEL_H
#define FINMAN_ABSTRACTMODEL_H


#include <vector>
#include <string>
#include <db/AbstractQueryBuilder.h>
#include <db/QueryResult.h>
#include "view/AbstractView.h"


/**
 * Abstract Model
 *
 * Provides an API for interacting with objects in the database.
 *
 * Create   a static create() method should be implemented in the subclass
 * Read     Instantiation of the class queries the database for the object, and throws an error if the query is
 *          ambiguous.  Calling get_view will create a detail view for the object
 * Update   calling update will update the specified columns with the values provided. Each pair should consist of
 *          a <column, value> pair to set.
 * Delete   del() will delete the object from the database, or set the object to 'inactive' depending on the object
 */
class AbstractModel
{
public:
    explicit AbstractModel(AbstractQueryBuilder* builder) : _builder(builder) {};

    // static create();
    virtual QueryResult* get();
    virtual bool update(std::vector<std::pair<std::string,std::string>> set);
    virtual void del();
    virtual AbstractView* get_view();

    ~AbstractModel() = default;

protected:
    AbstractQueryBuilder* _builder;
};

#endif //FINMAN_ABSTRACTMODEL_H
