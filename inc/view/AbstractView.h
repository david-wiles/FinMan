#ifndef FINMAN_ABSTRACTVIEW_H
#define FINMAN_ABSTRACTVIEW_H


#include "db/QueryResult.h"


class AbstractView
{
public:
    AbstractView() : _obj(nullptr) {};
    explicit AbstractView(QueryResult* obj) : _obj(obj){};

    virtual void print() = 0;

    ~AbstractView() = default;

protected:
    QueryResult* _obj;
};


#endif //FINMAN_ABSTRACTVIEW_H
