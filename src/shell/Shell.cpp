#include <shell/Shell.h>
#include <sstream>
#include <iostream>


void Shell::loop(const char* username)
{
    int status;
    std::string auth_user(username);

    do {
        std::cout << "FinMan > ";
        status = Shell::execute(auth_user, Shell::get_args());
    } while (status);

}

std::vector<std::string> Shell::get_args()
{
    char* input = nullptr;
    std::vector<std::string> args;

    // Read a line of input from console using GNU readline
    input = readline(input);
    while (strlen(input) == 0)
        input = readline(input);

    add_history(input);

    std::string input_str(input);
    std::stringstream ss(input_str);

    // Split line into words
    std::string token;
    while (ss >> token)
        args.push_back(token);

    return args;
}

int Shell::execute(const std::string username, const std::vector<std::string> args)
{
    if (args.empty())
        return 1;

    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (args[0] == Shell::commands_str[i]) {
            return (*Shell::commands[i])(username, args);
        }
    }

    return 0;
}
