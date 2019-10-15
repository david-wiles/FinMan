#ifndef FINMAN_ASSETVIEW_H
#define FINMAN_ASSETVIEW_H


#include "AbstractView.h"

class AssetView : public AbstractView
{
public:
    AssetView() : AbstractView() {};
    explicit AssetView(QueryResult* obj) : AbstractView(obj) {};

    void print() override;
};


#endif //FINMAN_ASSETVIEW_H
