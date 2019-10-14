#ifndef FINMAN_TRANSACTIONVIEW_H
#define FINMAN_TRANSACTIONVIEW_H


#include <view/AbstractView.h>
#include <db/QueryResult.h>

class TransactionView : public AbstractView
{
public:
    TransactionView() : AbstractView() {};
    explicit TransactionView(QueryResult* obj) : AbstractView(obj) {};

    void print() override;
};


#endif //FINMAN_TRANSACTIONVIEW_H
