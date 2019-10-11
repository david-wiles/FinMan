#ifndef FINMAN_ABSTRACTMODEL_H
#define FINMAN_ABSTRACTMODEL_H


#include <vector>
#include <string>
#include <db/AbstractQueryBuilder.h>
#include <db/QueryResult.h>
#include "view/AbstractView.h"


class AbstractModel
{
public:
    explicit AbstractModel(AbstractQueryBuilder* builder) : _obj(nullptr), _builder(nullptr) {};

    // static create();
    void update(std::vector<std::pair<std::string,std::string>> set);
    virtual void del();
    virtual AbstractView* get_view() = 0;

    ~AbstractModel()
    {
        delete(_obj);
        delete(_builder);
    }

protected:
    QueryResult* _obj;
    AbstractQueryBuilder* _builder;
};

#endif //FINMAN_ABSTRACTMODEL_H
