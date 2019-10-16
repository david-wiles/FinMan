#ifndef FINMAN_BUDGET_H
#define FINMAN_BUDGET_H


#include "AbstractModel.h"

class Budget : public AbstractModel
{
public:
    explicit Budget(AbstractQueryBuilder *builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);

    ~Budget() = default;
};


#endif //FINMAN_BUDGET_H
