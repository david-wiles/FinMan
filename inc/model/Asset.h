#ifndef FINMAN_ASSET_H
#define FINMAN_ASSET_H


#include "AbstractModel.h"

class Asset : public AbstractModel
{
public:
    explicit Asset(SQLite3QueryBuilder* builder) : AbstractModel(builder) {};

    static bool create(const std::vector<std::string>& vals);

    ~Asset() = default;
};


#endif //FINMAN_ASSET_H
