#ifndef FINMAN_ACCOUNT_H
#define FINMAN_ACCOUNT_H


#include "AbstractModel.h"


/**
 * Model for Accounts.
 *
 * Allows all basic operations.
 */
class Account : public AbstractModel
{
public:
    explicit Account(AbstractQueryBuilder *builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);
    void del() override;

    ~Account() = default;
};


#endif //FINMAN_ACCOUNT_H
