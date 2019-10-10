#include <shell/Shell.h>
#include <sstream>
#include <iostream>
#include <controller/Controller.h>


void Shell::loop()
{
    int status = 0;

    do {
        status = this->execute(Shell::get_args());
    } while (status == 0);

}

std::vector<std::string>* Shell::get_args()
{
    char* input = nullptr;
    auto* args = new std::vector<std::string>();

    // Read a line of input from console using GNU readline
    input = readline(const_cast<char *>(this->_prompt.c_str()));
    if (strlen(input) != 0)
        add_history(input);

    std::string input_str(input);
    std::stringstream ss(input_str);

    // Split line into words
    std::string token;
    while (ss >> token)
        args->push_back(token);

    return args;
}

int Shell::execute(const std::vector<std::string>* args)
{
    if (args->empty())
        return 0;

    return Controller::execute(this->_username, args);

}
