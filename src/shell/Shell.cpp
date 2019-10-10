#include <shell/Shell.h>
#include <sstream>
#include <iostream>


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

int hello(const std::string username, const std::vector<std::string>* unused_params)
{
    std::cout << "Hello, " << username << "!" << std::endl;
    return 0;
}

int goodbye(const std::string unused_user, const std::vector<std::string>* unused_params)
{
    std::cout << "Goodbye" << std::endl;
    return 1;
}

const std::string commands_str[] = {
    std::string("hello"),
    std::string("goodbye")
};

int (*commands[NUM_COMMANDS]) (const std::string, const std::vector<std::string>*) = {
    &hello,
    &goodbye,
};

int Shell::execute(const std::vector<std::string>* args)
{
    if (args->empty())
        return 0;

    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (args->at(0) == commands_str[i]) {
            return (*commands[i])(this->_username, args);
        }
    }

    return 0;
}

