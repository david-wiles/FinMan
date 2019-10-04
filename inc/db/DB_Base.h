#ifndef FINMAN_DB_BASE_H
#define FINMAN_DB_BASE_H


class DB_Base
{
public:
    DB_Base() = default;
    virtual void init_db() = 0;
    virtual ~DB_Base() = default;
};

#endif //FINMAN_DB_BASE_H
