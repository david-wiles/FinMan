#ifndef FINMAN_CONTROLLER_H
#define FINMAN_CONTROLLER_H


#include <string>
#include <vector>

namespace Controller
{
    // Commands available to the user
    int hello(const std::string& username, const std::vector<std::string>* unused_params);
    int goodbye(const std::string& unused_user, const std::vector<std::string>* unused_params);
    int help(const std::string& unused_user, const std::vector<std::string>* params);
    int account(const std::string& username, const std::vector<std::string>* params);
    int transaction(const std::string& username, const std::vector<std::string>* params);
    int assets(const std::string& username, const std::vector<std::string>* params);
    int income(const std::string& username, const std::vector<std::string>* params);
    int investments(const std::string& username, const std::vector<std::string>* params);
    int budget(const std::string& username, const std::vector<std::string>* params);

    extern std::vector<std::string> cmd_str_arr;
    extern int (*cmds[]) (const std::string&, const std::vector<std::string>*);

}


#endif //FINMAN_CONTROLLER_H
