#include <iostream>
#include <db/QueryResult.h>
#include <db/SQLite3Instance.h>
#include <db/SQLite3QueryBuilder.h>
#include "controller/Controller.h"
#include <view/TableView.h>
#include <model/Account.h>
#include <model/Transaction.h>
#include <ctime>
#include <view/AccountView.h>
#include <util.h>
#include <fstream>
#include <sstream>


std::vector<std::string> Controller::cmd_str_arr = {
        "hello",
        "goodbye",
        "help",
        "account",
        "assets",
        "income",
        "investments",
        "budget",
        "overview"
};

int (*Controller::cmds[]) (const std::string&, const std::vector<std::string>*) = {
        &hello,
        &goodbye,
        &help,
        &account,
        &transaction,
        &assets,
        &income,
        &investments,
        &budget,
        &overview
};


int error_msg(const std::string& err)
{
    std::cout << err << std::endl;
    std::cout << "Type 'help' to see a list of valid commands and options." << std::endl;
    return 0;
}

std::string get_param(const std::vector<std::string>* params, int index)
{
    try {
        return params->at(index);
    } catch (std::out_of_range &err) {
        return std::string();
    }
}


int Controller::hello(const std::string& username, const std::vector<std::string>* unused_params)
{
    std::cout << "Hello, " << username << "!" << std::endl;

    delete(unused_params);
    return 0;
}

int Controller::goodbye(const std::string& unused_user, const std::vector<std::string>* unused_params)
{
    std::cout << "Goodbye" << std::endl;
    return 1;
}

int Controller::help(const std::string& unused_user, const std::vector<std::string>* params)
{
    return 0;
}


int new_account(const std::string& username)
{
    std::string custodian;
    std::string acct_num_str;
    std::string balance;
    std::string type;
    std::string interest;
    std::string name;

    std::cout << "Enter a custodian for this account (press enter if none): " << std::endl;
    std::cin >> custodian;

    std::cout << "Enter this account's account number: " << std::endl;
    std::cin >> acct_num_str;

    std::cout << "Enter the balance currently in the account: " << std::endl;
    std::cin >> balance;

    std::cout << "Enter the type of account (checking, savings, retirement, etc.): " << std::endl;
    std::cin >> type;

    std::cout << "Enter the interest rate for the account (press enter if 0): " << std::endl;
    std::cin >> interest;

    std::cout << "Enter a name for the account(optional): " << std::endl;
    std::cin >> name;

    if (Account::create({acct_num_str, username, custodian, balance, type, interest, name}))
        return 0;
    else
        return error_msg("Could not create account: ");
}

int Controller::account(const std::string& username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));
    if (action == "view") {

        std::string acct(get_param(params, 2));
        auto* query = new SQLite3QueryBuilder("account");
        auto* view = new AccountView();

        if (!acct.empty()) {

            // Select account where owner or custodian is user and acct_num is acc
            query
            ->select({})
            ->opt_where({
                std::make_pair("owner", username),
                std::make_pair("custodian", username)
            })->where(std::make_pair("acct_num", acct));

        } else {

            // Select account where owner or custodian is user
            query
            ->select({})
            ->opt_where({
                std::make_pair("owner", username),
                std::make_pair("custodian", username)
            });

        }

        /*
        Account account(query);
        view = account.get_view();
        view->print();
        */

        delete(query);
        delete(view);

        return 0;

    } else if (action == "create") {

        // User is prompted with fields for account
        return new_account(username);

    } else if (action == "modify") {

        std::string acct(get_param(params, 3));

        if (!acct.empty()) {

            std::string opt(get_param(params, 4));
            std::string col(get_param(params, 5));

            if (!opt.empty()) {

                std::string val;
                if (opt == "-u")
                    val = get_param(params, 6);
                else if (opt != "-r") {
                    error_msg("Invalid option for account.");
                    return 0;
                }

                // TODO determine cols

                // Update account where owner or custodian is user and acct_num is acct
                auto* query = new SQLite3QueryBuilder("account");
                query
                ->opt_where({
                    std::make_pair("owner", username),
                    std::make_pair("custodian", username)
                })
                ->where(std::make_pair("acct_num", acct));

                Account account(query);
                if (!account.update({std::make_pair(col, val)}))
                    return error_msg("Could not update account.");
                return 0;

            } else
                return error_msg("An option must be used with modify: -u to update a column or -r to remove the data in the column.");

        } else
            return error_msg("An account number must be provided to update and account.");

    } else
        return error_msg("Could not perform the requested action with given arguments.");
}


int Controller::transaction(const std::string& username, const std::vector<std::string>* params)
{
    std::string acct = get_param(params, 1);
    std::string action = get_param(params, 2);

    if (acct.empty())
        return error_msg("An account number must be provided to perform a transaction.");

    if (action == "-f") {

        std::string filename = get_param(params, 3);

        // Insert from a list of transactions
        std::ifstream transact_f(filename);
        std::string line;
        auto* transactions = new std::vector<std::vector<std::string>>();

        while (std::getline(transact_f, line)) {
            std::string token;
            std::stringstream ss(line);
            std::vector<std::string> tokens;

            while (ss >> token)
                tokens.push_back(token);

            transactions->push_back(tokens);
        }

        auto* query = new SQLite3QueryBuilder("transaction");
        query
        ->insert({"type", "amount", "from_acct", "to_acct", "date"})
        ->values(transactions);

        if (SQLite3Instance::getInstance()->query(query) == nullptr)
            return error_msg("Could not create transactions. Check the format of the file and try again.");

        delete(transactions);
        delete(query);

        return 0;

    } else if (action == "withdraw") {

        std::string amt = get_param(params, 3);
        std::string description = get_param(params, 4);

        if (amt.empty())
            return error_msg("An amount must be provided to make a withdrawal.");

        if (!Transaction::create({"withdraw", amt, acct, "", util::now()}))
            return error_msg("Could not make withdrawal.");

        return 0;

    } else if (action == "deposit") {

        std::string amt = get_param(params, 3);
        std::string description = get_param(params, 4);

        if (amt.empty())
            return error_msg("An amount must be provided to make a deposit.");

        if (!Transaction::create({"deposit", amt, "", acct, util::now()}))
            return error_msg("Could not make deposit.");

        return 0;

    } else if (action == "transfer") {

        std::string to_acct = get_param(params, 3);
        std::string amt = get_param(params, 4);

        if (to_acct.empty())
            return error_msg("A destination account must be specified.");
        if (amt.empty())
            return error_msg("An amount must be specified.");

        if (!Transaction::create({"transfer", amt, acct, to_acct, util::now()}))
            return error_msg("Could not complete transfer.");

        return 0;

    } else
        return error_msg("Could not perform the requested action.");

}


int Controller::assets(const std::string& username, const std::vector<std::string>* params)
{
    return 0;
}


int new_income(const std::string& username)
{
    return 0;
}

int Controller::income(const std::string& username, const std::vector<std::string>* params)
{
    std::string action = get_param(params, 1);

    if (action == "view") {

        std::string id = get_param(params, 2);

        // View incomes associated with user
        return 0;

    } else if (action == "add") {

        return new_income(username);

    } else if (action == "remove") {

        std::string id = get_param(params, 2);

        if (id.empty())
            return error_msg("An income id must be provided in order to remove an income. Use the command 'income view' to view the information about all of your incomes.");

        // Remove income
        return 0;

    } else
        return error_msg("Could not perform the requested action with given arguments.");

}


int Controller::investments(const std::string& username, const std::vector<std::string>* params)
{
    return 0;
}


int Controller::budget(const std::string& username, const std::vector<std::string>* params)
{
    return 0;
}

int Controller::overview(const std::string &username, const std::vector<std::string> *params)
{
    return 0;
}