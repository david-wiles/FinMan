#ifndef FINMAN_SHELL_H
#define FINMAN_SHELL_H

#include <string>
#include <utility>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#define NUM_COMMANDS 5


class Shell
{
public:
    explicit Shell(std::string username) : _username(std::move(username)) {};

    /**
    * Shell main loop
    *
    * @param username The username to pass as an argument to commands requiring authentication
    */
    void loop();

private:
    // authenticated user's username
    std::string _username;
    /**
    * Read a line of input from console and split into string tokens to pass to execute
    *
    * @return Array of arguments
    */
    static std::vector<std::string> get_args();

    /**
    * Execute a command passed
    *
    * @param username The username to pass as an argument to commands requiring authentication
    * @param args Array of arguments
    * @return Return status of the executed command
    */
    int execute(const std::vector<std::string>& args);

    const std::string commands_str[NUM_COMMANDS];
    const int (*commands[NUM_COMMANDS]) (std::string, std::vector<std::string>){};

    // TODO commands
};

#endif //FINMAN_SHELL_H
