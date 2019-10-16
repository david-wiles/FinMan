#ifndef FINMAN_INCOME_H
#define FINMAN_INCOME_H


#include <model/AbstractModel.h>
#include <db/SQLite3QueryBuilder.h>

class Income : public AbstractModel
{
public:
    explicit Income(SQLite3QueryBuilder* builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);

    ~Income() = default;
};


#endif //FINMAN_INCOME_H
