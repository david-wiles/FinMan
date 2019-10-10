#include <shell/Shell.h>
#include <sstream>
#include <controller/Controller.h>


Shell::Shell(std::string username) : _username(username)
{
    // Print welcome message once user has been authenticated
    std::string welcome("Hello");
    std::cout << welcome << std::endl;
    this->_prompt = username += "@FinMan > ";
}

void Shell::loop()
{
    int status = 0;

    do {
        status = Shell::execute(this->_username, Shell::get_args());
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

int Shell::execute(std::string username, const std::vector<std::string>* args)
{
    int i = 0;
    for (auto &itr: Controller::cmd_str_arr) {
        if (args->at(0) == itr) {
            return (*Controller::cmds[i])(username, args);
        }
        i++;
    }

    // TODO Print command not found message

    return 0;
}
