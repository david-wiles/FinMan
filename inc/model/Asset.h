#ifndef FINMAN_ASSET_H
#define FINMAN_ASSET_H


#include "AbstractModel.h"

class Asset : public AbstractModel
{
public:
    explicit Asset(AbstractQueryBuilder* builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);
    AbstractView* get_view() override;
    void del() override;

    ~Asset() = default;
};


#endif //FINMAN_ASSET_H
