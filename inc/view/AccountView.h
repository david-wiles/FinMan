#ifndef FINMAN_ACCOUNTVIEW_H
#define FINMAN_ACCOUNTVIEW_H


#include "AbstractView.h"

class AccountView : public AbstractView
{
public:
    AccountView() : AbstractView() {};
    explicit AccountView(QueryResult* obj) : AbstractView(obj) {};

    void print() override;
};


#endif //FINMAN_ACCOUNTVIEW_H
