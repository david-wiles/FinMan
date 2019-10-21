#include <fstream>
#include <iostream>
#include "view/StaticView.h"

void StaticView::from_file(const std::string& filename)
{
    std::ifstream f(filename);
    if (f.is_open())
        std::cout << f.rdbuf();
    f.close();
}
