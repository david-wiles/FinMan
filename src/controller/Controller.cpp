#include <iostream>
#include <db/QueryResult.h>
#include <db/SQLite3Instance.h>
#include <db/SQLite3QueryBuilder.h>
#include "controller/Controller.h"
#include <view/TableView.h>
#include <model/Account.h>
#include <model/Transaction.h>
#include <ctime>
#include <util.h>
#include <fstream>
#include <sstream>
#include <model/Asset.h>
#include <model/Income.h>
#include <model/Debt.h>
#include <model/Investment.h>
#include <model/Budget.h>


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
        &debt,
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


// Management functions

int new_account(const std::string& username)
{
    std::string custodian;
    std::string acct_num_str;
    std::string balance;
    std::string type;
    std::string interest;
    bool valid = false;

    std::cout << "Enter a custodian for this account (press enter if none): " << std::endl;
    std::cin >> custodian;

    if (custodian.empty()) {
        custodian = username;
    }

    while (!valid) {
        std::cout << "Enter this account's account number: " << std::endl;
        std::cin >> acct_num_str;

        try {
            std::stoi(acct_num_str);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Account number must only contain numbers." << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the balance currently in the account: " << std::endl;
        std::cin >> balance;

        try {
            std::stod(balance);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Account balance must be a valid decimal value." << std::endl;
        }
    }

    std::cout << "Enter the type of account (checking, savings, retirement, etc.): " << std::endl;
    std::cin >> type;

    valid = false;
    while (!valid) {
        std::cout << "Enter the interest rate for the account: " << std::endl;
        std::cin >> interest;

        try {
            std::stod(interest);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Interest rate must be a valid decimal value." << std::endl;
        }
    }

    if (Account::create({acct_num_str, username, custodian, balance, type, interest}))
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

        auto* res = SQLite3Instance::getInstance()->query(query);
        TableView::view(res);

        return 0;

    } else if (action == "create") {

        // User is prompted with fields for account
        return new_account(username);

    } else if (action == "modify") {

        std::string acct(get_param(params, 3));

        if (!acct.empty()) {

            std::string opt(get_param(params, 4));
            std::string col(get_param(params, 5));
            auto* query = new SQLite3QueryBuilder("account");
            // Update account where owner or custodian is user and acct_num is acct
            query
            ->opt_where({
                std::make_pair("owner", username),
                std::make_pair("custodian", username)
            })
            ->where(std::make_pair("acct_num", acct));
            Account account(query);

            if (!opt.empty()) {

                std::string val = get_param(params, 6);
                bool success;

                if (opt == "--owner")
                    success = account.update({std::make_pair("owner", val)});
                else if (opt == "--custodian")
                    success = account.update({std::make_pair("custodian", val)});
                else
                    return error_msg("Could not modify account with the given option.");

                if (success)
                    return 0;
                else
                    return error_msg("Could not update account.");

            } else
                return error_msg("An attribute to modify must be specified: --owner to change owner, --custodian to change custodian.");

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


int new_asset(const std::string& username)
{
    return 0;
}

int Controller::assets(const std::string& username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));

    if (action == "view") {

        std::string id(get_param(params, 2));
        auto* query = new SQLite3QueryBuilder("assets");

        if (id.empty()) {
            query
            ->select({})
            ->where(std::make_pair("owner", username));
        } else {
            query
            ->select({})
            ->where({std::make_pair("owner", username), std::make_pair("name", id)});
        }

        auto* res = SQLite3Instance::getInstance()->query(query);
        TableView::view(res);

        return 0;

    } else if (action == "add") {

        return new_asset(username);

    } else if (action == "remove") {

        std::string id(get_param(params, 2));
        auto* query = new SQLite3QueryBuilder("asset");
        query->where({std::make_pair("owner", username), std::make_pair("name", id)});

        Asset asset(query);
        asset.del();

        return 0;
    } else if (action == "modify") {

        std::string id(get_param(params, 2));
        std::string opt(get_param(params, 3));
        std::string val(get_param(params, 4));

        auto query = new SQLite3QueryBuilder("asset");
        query->where({std::make_pair("owner", username), std::make_pair("id", id)});
        // TODO determine cols

        Asset asset(query);

        if (asset.get()) {
            if (asset.update({std::make_pair("col", val)}))
                return 0;
            else
                return error_msg("Could not update asset.");
        } else {
            return error_msg("Could not find asset.");
        }

    } else
        return error_msg("Could not interpret commands.");
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
        auto* query = new SQLite3QueryBuilder("income");

        if (id.empty()) {
            query
            ->select({})
            ->where(std::make_pair("owner", username));
        } else {
            query
            ->select({})
            ->where({std::make_pair("owner", username), std::make_pair("name", id)});
        }

        auto* res = SQLite3Instance::getInstance()->query(query);
        TableView::view(res);

        return 0;

    } else if (action == "add") {

        return new_income(username);

    } else if (action == "remove") {

        std::string id = get_param(params, 2);

        if (id.empty())
            return error_msg("An income name must be provided in order to remove an income. Use the command 'income view' to view the information about all of your incomes.");

        auto* query = new SQLite3QueryBuilder("income");
        query->where({std::make_pair("owner", username), std::make_pair("alias", id)});

        Income income(query);
        income.del();

        return 0;

    } else if (action == "modify") {

        std::string id(get_param(params, 2));
        std::string opt(get_param(params, 3));
        std::string val(get_param(params, 4));

        // TODO validate col / opt

        auto query = new SQLite3QueryBuilder("income");
        query->where({std::make_pair("owner", username), std::make_pair("alias", id)});

        Income income(query);

        if (income.get()) {
            if (income.update({std::make_pair("col", val)}))
                return 0;
            else
                return error_msg("Could not update income.");
        } else
            return error_msg("Could not find specified income.");

    } else
        return error_msg("Could not perform the requested action with given arguments.");

}


int new_debt(const std::string& username)
{
    return 0;
}

int Controller::debt(const std::string& username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));

    if (action == "view") {

        std::string id(get_param(params, 2));
        auto query = new SQLite3QueryBuilder("debt");

        if (id.empty()) {
            query->select({})->where(std::make_pair("owner", username));
        } else {
            query->select({})->where({std::make_pair("owner", username), std::make_pair("id", id)});
        }

        TableView::view(SQLite3Instance::getInstance()->query(query));
        return 0;
    } else if (action == "add") {

        return new_debt(username);

    } else if (action == "remove") {

        std::string id(get_param(params, 2));
        auto query = new SQLite3QueryBuilder("debt");

        if (id.empty())
            return error_msg("A specific debt must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Debt debt(query);

        if (debt.get()) {
            debt.del();
            return 0;
        } else
            return error_msg("The debt could not be found.");

    } else if (action == "modify") {

        std::string id(get_param(params, 2));
        std::string opt(get_param(params, 3));
        std::string val(get_param(params, 4));

        // TODO validation

        auto query = new SQLite3QueryBuilder("debt");

        if (id.empty())
            return error_msg("A specific debt must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Debt debt(query);

        if (debt.get()) {
            if (debt.update({std::make_pair("col", val)}))
                return 0;
            else
                return error_msg("Could not update the debt.");
        } else
            return error_msg("Could not find the debt.");
    } else
        return error_msg("Invalid command.");
}

int new_investment(const std::string& username)
{
    return 0;
}

int Controller::investments(const std::string& username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));

    if (action == "view") {

        std::string id(get_param(params, 2));
        auto query = new SQLite3QueryBuilder("investment");

        if (id.empty()) {
            query->select({})->where(std::make_pair("owner", username));
        } else {
            query->select({})->where({std::make_pair("owner", username), std::make_pair("id", id)});
        }

        TableView::view(SQLite3Instance::getInstance()->query(query));
        return 0;
    } else if (action == "add") {

        return new_investment(username);

    } else if (action == "remove") {

        std::string id(get_param(params, 2));
        auto query = new SQLite3QueryBuilder("investment");

        if (id.empty())
            return error_msg("A specific investment must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Investment investment(query);

        if (investment.get()) {
            investment.del();
            return 0;
        } else
            return error_msg("The investment could not be found.");

    } else if (action == "modify") {

        std::string id(get_param(params, 2));
        std::string opt(get_param(params, 3));
        std::string val(get_param(params, 4));

        // TODO validation

        auto query = new SQLite3QueryBuilder("investment");

        if (id.empty())
            return error_msg("A specific investment must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Investment investment(query);

        if (investment.get()) {
            if (investment.update({std::make_pair("col", val)}))
                return 0;
            else
                return error_msg("Could not update the investment.");
        } else
            return error_msg("Could not find the investment.");
    } else
        return error_msg("Invalid command.");
}


int new_budget(const std::string& username)
{
    return 0;
}

int Controller::budget(const std::string& username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));

    if (action == "view") {

        std::string id(get_param(params, 2));
        auto query = new SQLite3QueryBuilder("budget");

        if (id.empty()) {
            query->select({})->where(std::make_pair("owner", username));
        } else {
            query->select({})->where({std::make_pair("owner", username), std::make_pair("id", id)});
        }

        TableView::view(SQLite3Instance::getInstance()->query(query));
        return 0;
    } else if (action == "add") {

        return new_budget(username);

    } else if (action == "remove") {

        std::string id(get_param(params, 2));
        auto query = new SQLite3QueryBuilder("budget");

        if (id.empty())
            return error_msg("A specific budget must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Budget budget(query);

        if (budget.get()) {
            budget.del();
            return 0;
        } else
            return error_msg("The budget could not be found.");

    } else if (action == "modify") {

        std::string id(get_param(params, 2));
        std::string opt(get_param(params, 3));
        std::string val(get_param(params, 4));

        // TODO validation

        auto query = new SQLite3QueryBuilder("budget");

        if (id.empty())
            return error_msg("A specific budget must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Budget budget(query);

        if (budget.get()) {
            if (budget.update({std::make_pair("col", val)}))
                return 0;
            else
                return error_msg("Could not update the budget.");
        } else
            return error_msg("Could not find the budget.");
    } else
        return error_msg("Invalid command.");
}

int Controller::overview(const std::string &username, const std::vector<std::string> *params)
{
    return 0;
}