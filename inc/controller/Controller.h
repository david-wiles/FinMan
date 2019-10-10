#ifndef FINMAN_CONTROLLER_H
#define FINMAN_CONTROLLER_H


#include <string>
#include <vector>

namespace Controller
{
    // Commands available to the user
    int hello(std::string username, const std::vector<std::string>* unused_params);
    int goodbye(std::string unused_user, const std::vector<std::string>* unused_params);
    int help(std::string unused_user, const std::vector<std::string>* params);
    int account(std::string username, const std::vector<std::string>* params);
    int transaction(std::string username, const std::vector<std::string>* params);
    int assets(std::string username, const std::vector<std::string>* params);
    int income(std::string username, const std::vector<std::string>* params);
    int investments(std::string username, const std::vector<std::string>* params);
    int budget(std::string username, const std::vector<std::string>* params);

    std::string cmd_str_arr[] = {
            "hello",
            "goodbye",
            "help",
            "account",
            "assets",
            "income",
            "investments",
            "budget"
    };

    int (*cmds[]) (std::string, const std::vector<std::string>*) = {
            &hello,
            &goodbye,
            &help,
            &account,
            &transaction,
            &assets,
            &income,
            &investments,
            &budget
    };

}


#endif //FINMAN_CONTROLLER_H
