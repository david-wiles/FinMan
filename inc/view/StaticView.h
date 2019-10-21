#ifndef FINMAN_STATICVIEW_H
#define FINMAN_STATICVIEW_H


#include "AbstractView.h"

class StaticView : public AbstractView
{
public:
    StaticView() : AbstractView() {};

    static void from_file(const std::string& filename);

    ~StaticView() = default;
};


#endif //FINMAN_STATICVIEW_H
