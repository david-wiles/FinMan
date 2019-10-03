#ifndef FINMAN_SHELL_H
#define FINMAN_SHELL_H

#include <string>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#define NUM_COMMANDS 5


namespace Shell
{
    /**
    * Shell main loop
    *
    * @param username The username to pass as an argument to commands requiring authentication
    */
    void loop(const char* username);

    /**
    * Read a line of input from console and split into string tokens to pass to execute
    *
    * @return Array of arguments
    */
    std::vector<std::string> get_args();

    /**
    * Execute a command passed
    *
    * @param username The username to pass as an argument to commands requiring authentication
    * @param args Array of arguments
    * @return Return status of the executed command
    */
    int execute(std::string username, std::vector<std::string> args);

    const std::string commands_str[NUM_COMMANDS];
    const int (*commands[NUM_COMMANDS]) (std::string, std::vector<std::string>);

    // TODO commands
};

#endif //FINMAN_SHELL_H
