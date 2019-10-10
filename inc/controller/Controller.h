#ifndef FINMAN_CONTROLLER_H
#define FINMAN_CONTROLLER_H


#include <string>
#include <vector>

#define NUM_COMMANDS 2

namespace Controller
{
    int execute(std::string username, const std::vector<std::string>* args);

    int hello(std::string username, const std::vector<std::string>* unused_params);
    int goodbye(std::string unused_user, const std::vector<std::string>* unused_params);

    std::string cmd_str_arr[] = {
            "hello",
            "goodbye"
    };

    int (*cmds[]) (std::string, const std::vector<std::string>*) = {
            &hello,
            &goodbye,
    };

}


#endif //FINMAN_CONTROLLER_H
