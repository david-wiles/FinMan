#ifndef FINMAN_SHELL_H
#define FINMAN_SHELL_H

#include <string>
#include <utility>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

#define NUM_COMMANDS 1


class Shell
{
public:
    explicit Shell(std::string username) : _username(username)
    {
        this->_prompt = username += "@FinMan > ";
    };

    /**
    * Shell main loop
    *
    * @param username The username to pass as an argument to commands requiring authentication
    */
    void loop();

private:
    // authenticated user's username
    std::string _username;
    std::string _prompt;
    /**
    * Read a line of input from console and split into string tokens to pass to execute
    *
    * @return Array of arguments
    */
    std::vector<std::string>* get_args();

    /**
    * Execute a command passed
    *
    * @param username The username to pass as an argument to commands requiring authentication
    * @param args Array of arguments
    * @return Return status of the executed command
    */
    int execute(const std::vector<std::string>* args);

    /**
     * Greet the user, if they say hello
     *
     * @param username  The logged in user
     * @param unused
     * @return          Return status
     */
    static int hello(std::string username, const std::vector<std::string>* unused);

    /**
     * Arrays to contain the possible commands a user can enter. One array contains string representation, and the
     * other contains function pointers.
     */
    static const std::string commands_str[];
    static int (*commands[]) (const std::string, const std::vector<std::string>*);

    // TODO commands
};

#endif //FINMAN_SHELL_H
