#ifndef FINMAN_INCOMEVIEW_H
#define FINMAN_INCOMEVIEW_H


#include <db/QueryResult.h>
#include "AbstractView.h"

class IncomeView : public AbstractView
{
public:
    IncomeView() : AbstractView() {};
    explicit IncomeView(QueryResult* obj) : AbstractView(obj) {};

    void print() override;
};


#endif //FINMAN_INCOMEVIEW_H
