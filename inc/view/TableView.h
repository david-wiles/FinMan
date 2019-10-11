#ifndef TEST_TABLEVIEW_H
#define TEST_TABLEVIEW_H


#include <db/QueryResult.h>

class TableView
{
public:
    TableView(QueryResult* res);

    void print();

private:
    QueryResult* _query_result;
};


#endif //TEST_TABLEVIEW_H
