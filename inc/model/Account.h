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
    explicit Account(AbstractQueryBuilder *builder);

    AbstractView* get_view() override;

    ~Account() = default;
};


#endif //FINMAN_ACCOUNT_H
