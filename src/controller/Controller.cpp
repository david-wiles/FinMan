#include <iostream>
#include "controller/Controller.h"


int Controller::execute(std::string username, const std::vector<std::string>* args)
{
    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (args->at(0) == Controller::cmd_str_arr[i]) {
            return (*Controller::cmds[i])(username, args);
        }
    }
    return 0;
}

int Controller::hello(std::string username, const std::vector<std::string>* unused_params)
{
    std::cout << "Hello, " << username << "!" << std::endl;
    return 0;
}

int Controller::goodbye(std::string unused_user, const std::vector<std::string>* unused_params)
{
    std::cout << "Goodbye" << std::endl;
    return 1;
}