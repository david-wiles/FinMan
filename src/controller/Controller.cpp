#include <iostream>
#include <db/QueryResult.h>
#include <db/SQLite3Instance.h>
#include <view/TableView.h>
#include "controller/Controller.h"


std::string get_param(const std::vector<std::string>* params, int index)
{
    try {
        return params->at(index);
    } catch (std::out_of_range &err) {
        // TODO print error message
    }

    return std::string();
}

int error_msg(std::string err)
{
    std::cout << err << std::endl;
    std::cout << "Type 'help' to see a list of valid commands and options." << std::endl;
    return 0;
}

int Controller::hello(std::string username, const std::vector<std::string>* unused_params)
{
    std::cout << "Hello, " << username << "!" << std::endl;

    delete(unused_params);
    return 0;
}

int Controller::goodbye(std::string unused_user, const std::vector<std::string>* unused_params)
{
    std::cout << "Goodbye" << std::endl;

    delete(unused_params);
    return 1;
}

int new_account(const std::string username)
{
    return 0;
}

int Controller::account(std::string username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));
    if (action == "view") {

        std::string acct(get_param(params, 2));
        QueryResult* res;

        if (!acct.empty()) {

            // Select account where owner or custodian is user and acct_num is acct

        } else {

            // Select account where owner or custodian is user

        }

        TableView results(res);
        results.print();
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
                bool query_result = true;

                // Update account where owner or custodian is user and acct_num is acct

                if (query_result)
                    return 0;
                else {
                    error_msg("Could not update account.");
                    return 0;
                }

            } else
                return error_msg("An option must be used with modify: -u to update a column or -r to remove the data in the column.");

        } else
            return error_msg("An account number must be provided to update and account.");

    } else
        return error_msg("Could not perform the requested action with given arguments.");
}

int transaction(std::string username, const std::vector<std::string>* params)
{
    std::string action = get_param(params, 1);
    std::string acct = get_param(params, 2);

    if (acct.empty())
        return error_msg("An account number must be provided to perform a transaction.");

    if (action == "withdraw") {

        std::string amt = get_param(params, 3);
        std::string description = get_param(params, 4);

        if (amt.empty())
            return error_msg("An amount must be provided to make a withdrawal.");

        // Insert a transaction with given arguments and deduct from account
        return 0;

    } else if (action == "deposit") {

        std::string amt = get_param(params, 3);
        std::string description = get_param(params, 4);
        std::string date = get_param(params, 5);

        if (amt.empty())
            return error_msg("An amount must be provided to make a deposit.");

        // Insert transaction and add amount to account
        return 0;


    } else if (action == "transfer") {

        std::string to_acct = get_param(params, 3);
        std::string amt = get_param(params, 4);

        if (to_acct.empty())
            return error_msg("A destination account must be specified.");
        if (amt.empty())
            return error_msg("An amount must be specified.");

        // Insert transaction and update amounts in accounts
        return 0;


    } else
        return error_msg("Could not perform the requested action with given arguments.");

}

int assets(std::string username, const std::vector<std::string>* params);

int new_income(const std::string username)
{
    return 0;
}

int income(std::string username, const std::vector<std::string>* params)
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

int investments(std::string username, const std::vector<std::string>* params);
int budget(std::string username, const std::vector<std::string>* params);