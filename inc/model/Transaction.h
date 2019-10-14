#ifndef FINMAN_TRANSACTION_H
#define FINMAN_TRANSACTION_H


#include "AbstractModel.h"

class Transaction : public AbstractModel
{
public:
    explicit Transaction(AbstractQueryBuilder *builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);
    AbstractView* get_view() override;
    void del() override;

    ~Transaction() = default;

};


#endif //FINMAN_TRANSACTION_H
