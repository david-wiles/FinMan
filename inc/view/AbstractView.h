#ifndef FINMAN_ABSTRACTVIEW_H
#define FINMAN_ABSTRACTVIEW_H


#include "db/QueryResult.h"


class AbstractView
{
public:
    explicit AbstractView(QueryResult* obj) : _obj(obj){};

    virtual void print() = 0;

    ~AbstractView() { delete(_obj); }

protected:
    QueryResult* _obj;
};


#endif //FINMAN_ABSTRACTVIEW_H
