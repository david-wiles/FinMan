#include <shell/Shell.h>
#include <sstream>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <controller/Controller.h>
#include <db/SQLite3QueryBuilder.h>
#include <db/SQLite3Instance.h>
#include <model/Transaction.h>
#include <ctime>
#include <iomanip>


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

    update();

    do {
        status = Shell::execute(this->_username, Shell::get_args());
    } while (status == 0);

}

std::vector<std::string> Shell::get_args()
{
    char* input = nullptr;
    std::vector<std::string> args;
    // Read a line of input from console using GNU readline
    input = readline(const_cast<char *>(this->_prompt.c_str()));
    if (strlen(input) != 0)
        add_history(input);

    std::string input_str(input);
    std::stringstream ss(input_str);
    {
        delete(_builder);
    }

    // Split line into words
    std::string token;
    while (ss >> token)
        args.push_back(token);

    return args;
}

int Shell::execute(const std::string& username, const std::vector<std::string>& args)
{
    if (!args.empty()) {
        int i = 0;
        for (auto &itr: Controller::cmd_str_arr) {
            if (args.at(0) == itr) {
                return (*Controller::cmds[i])(username, &args);
            }
            i++;
        }
    }

    return 0;
}

void Shell::update()
{
    // Get user's accounts
    auto query = new SQLite3QueryBuilder("account");
    query
    ->select({"acct_num", "balance"})
    ->opt_where({std::make_pair("owner", this->_username), std::make_pair("custodian", this->_username)});

    auto res = SQLite3Instance::getInstance()->query(query);

    // Iterate through accounts
    for (auto &acct : *res->get_rows()) {

        query = new SQLite3QueryBuilder("transaction");
        query
        ->select({"id", "amount", "to_acct", "from_acct", "date"})
        ->opt_where({std::make_pair("to_acct", acct.at(0)), std::make_pair("from_acct", acct.at(0))})
        ->where(std::make_pair("completed", "f"));

        // Iterate through transactions associated with account which aren't completed
        auto transactions = SQLite3Instance::getInstance()->query(query);
        SQLite3QueryBuilder* transaction_query;
        for (auto &t : *transactions->get_rows()) {

            std::time_t rawtime;
            std::time(&rawtime);
            std::tm* now = std::localtime(&rawtime);

            std::tm* transaction_tm = {};
            std::istringstream(t.at(4)) >> std::get_time(transaction_tm, "%Y-%m-%d-%H-%M-%S");

            if (now - transaction_tm > 0) {

                transaction_query = new SQLite3QueryBuilder("transaction");
                transaction_query
                        ->update(std::make_pair("completed", "t"))
                        ->where(std::make_pair("id", t.at(0)));

                int new_amt;
                // Deposit
                if (acct.at(0) == t.at(2))
                    new_amt = stoi(acct.at(1)) + stoi(t.at(1));
                // Withdrawal
                else
                    new_amt = stoi(acct.at(1)) - stoi(t.at(1));

                auto acct_query = new SQLite3QueryBuilder("account");
                acct_query
                ->update(std::make_pair("balance", std::to_string(new_amt)))
                ->where(std::make_pair("acct_num", acct.at(0)));

                if (SQLite3Instance::getInstance()->query(transaction_query) != nullptr)
                    SQLite3Instance::getInstance()->query(acct_query);


            }

        }

    }


}
