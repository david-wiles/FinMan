#ifndef FINMAN_DB_BASE_H
#define FINMAN_DB_BASE_H


#include "DB_Result.h"

class DB_Base
{
public:
    DB_Base() = default;
    virtual void init_db() = 0;
    virtual DB_Result* execute(std::string sql, std::vector<std::string> *params) = 0;
    virtual ~DB_Base() = default;
};

#endif //FINMAN_DB_BASE_H
