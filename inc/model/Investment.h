#ifndef FINMAN_INVESTMENT_H
#define FINMAN_INVESTMENT_H


#include <model/AbstractModel.h>

class Investment : public AbstractModel
{
public:
    explicit Investment(AbstractQueryBuilder *builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);
    AbstractView* get_view() override;
    void del() override;

    ~Investment() = default;
};


#endif //FINMAN_INVESTMENT_H
