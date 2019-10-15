#ifndef FINMAN_TABLEVIEW_H
#define FINMAN_TABLEVIEW_H


#include "view/AbstractView.h"


class TableView : public AbstractView
{
public:
    explicit TableView(QueryResult* table) : AbstractView(table) {};

    static void view(QueryResult* table);
    void print() override;
};


#endif //FINMAN_TABLEVIEW_H
