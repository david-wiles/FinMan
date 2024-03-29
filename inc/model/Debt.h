#ifndef FINMAN_DEBT_H
#define FINMAN_DEBT_H


#include "AbstractModel.h"

class Debt : public AbstractModel
{
public:
    explicit Debt(AbstractQueryBuilder *builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);

    ~Debt() = default;
};


#endif //FINMAN_DEBT_H
