#include "controller/Controller.h"

#include <iostream>
#include <ctime>
#include <util.h>
#include <fstream>
#include <sstream>
#include <regex>

#include <db/QueryResult.h>
#include <db/SQLite3Instance.h>
#include <db/SQLite3QueryBuilder.h>

#include <model/Asset.h>
#include <model/Income.h>
#include <model/Debt.h>
#include <model/Investment.h>
#include <model/Budget.h>
#include <model/Account.h>
#include <model/Transaction.h>

#include <view/TableView.h>


std::vector<std::string> Controller::cmd_str_arr = {
        "hello",
        "goodbye",
        "help",
        "account",
        "assets",
        "income",
        "investments",
        "budget",
        "overview",
        "family"
};

int (*Controller::cmds[]) (const std::string&, const std::vector<std::string>) = {
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
        &overview,
        &family
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


int Controller::hello(const std::string& username, const std::vector<std::string> unused_params)
{
    std::cout << "Hello, " << username << "!" << std::endl;
    return 0;
}

int Controller::goodbye(const std::string& unused_user, const std::vector<std::string> unused_params)
{
    std::cout << "Goodbye" << std::endl;
    return 1;
}

int Controller::help(const std::string& unused_user, const std::vector<std::string> params)
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

    std::cin.ignore();

    std::cout << "Enter the username of the custodian for this account (press enter if none): " << std::endl;
    std::getline(std::cin, custodian);

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

    std::cin.ignore();
    std::cout << "Enter the type of account (checking, savings, retirement, etc.): " << std::endl;
    std::getline(std::cin, type);

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

int Controller::account(const std::string& username, const std::vector<std::string> params)
{
    std::string action(get_param(&params, 1));

    if (action == "view") {

        std::string acct(get_param(&params, 2));

        auto query = new SQLite3QueryBuilder("account");

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

        auto res = SQLite3Instance::getInstance()->query(query);
        TableView::view(res);

        return 0;

    } else if (action == "create") {

        // User is prompted with fields for account
        return new_account(username);

    } else if (action == "modify") {

        std::string acct(get_param(&params, 3));

        if (!acct.empty()) {

            std::string opt(get_param(&params, 4));
            std::string col(get_param(&params, 5));
            auto query = new SQLite3QueryBuilder("account");
            // Update account where owner or custodian is user and acct_num is acct
            query
            ->opt_where({
                std::make_pair("owner", username),
                std::make_pair("custodian", username)
            })
            ->where(std::make_pair("acct_num", acct));
            Account account(query);

            if (!opt.empty()) {

                std::string val = get_param(&params, 6);
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


int Controller::transaction(const std::string& username, const std::vector<std::string> params)
{
    std::string acct = get_param(&params, 1);
    std::string action = get_param(&params, 2);

    if (acct.empty())
        return error_msg("An account number must be provided to perform a transaction.");

    if (action == "-f") {

        std::string filename = get_param(&params, 3);

        // Insert from a list of transactions
        std::ifstream transact_f(filename);
        std::string line;
        auto transactions = new std::vector<std::vector<std::string>>();

        while (std::getline(transact_f, line)) {
            std::string token;
            std::stringstream ss(line);
            std::vector<std::string> tokens;

            while (ss >> token)
                tokens.push_back(token);

            transactions->push_back(tokens);
        }

        auto query = new SQLite3QueryBuilder("transaction");
        query
        ->insert({"type", "amount", "from_acct", "to_acct", "date"})
        ->values(transactions);

        if (SQLite3Instance::getInstance()->query(query) == nullptr)
            return error_msg("Could not create transactions. Check the format of the file and try again.");

        return 0;

    } else if (action == "withdraw") {

        std::string amt = get_param(&params, 3);
        std::string description = get_param(&params, 4);

        if (amt.empty())
            return error_msg("An amount must be provided to make a withdrawal.");

        if (!Transaction::create({"withdraw", amt, acct, "", util::now()}))
            return error_msg("Could not make withdrawal.");

        return 0;

    } else if (action == "deposit") {

        std::string amt = get_param(&params, 3);
        std::string description = get_param(&params, 4);

        if (amt.empty())
            return error_msg("An amount must be provided to make a deposit.");

        if (!Transaction::create({"deposit", amt, "", acct, util::now()}))
            return error_msg("Could not make deposit.");

        return 0;

    } else if (action == "transfer") {

        std::string to_acct = get_param(&params, 3);
        std::string amt = get_param(&params, 4);

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
    std::string value;
    std::string type;
    std::string name;
    bool valid = false;

    std::cout << "Enter the type of asset: " << std::endl;
    std::getline(std::cin, type);

    std::cout << "Enter a name for the asset: " << std::endl;
    std::getline(std::cin, name);

    while (!valid) {
        std::cout << "Enter the value of the asset: " << std::endl;
        std::cin >> value;

        try {
            std::stod(value);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Asset value must be a decimal number." << std::endl;
        }
    }

    if (Asset::create({username, type, name, value}))
        return 0;
    else
        return error_msg("Could not create asset.");
}

int Controller::assets(const std::string& username, const std::vector<std::string> params)
{
    std::string action(get_param(&params, 1));

    if (action == "view") {

        std::string name(get_param(&params, 2));
        auto query = new SQLite3QueryBuilder("assets");

        if (name.empty()) {
            query
            ->select({})
            ->where(std::make_pair("owner", username));
        } else {
            query
            ->select({})
            ->where({std::make_pair("owner", username), std::make_pair("name", name)});
        }

        auto res = SQLite3Instance::getInstance()->query(query);
        TableView::view(res);

        return 0;

    } else if (action == "add") {

        return new_asset(username);

    } else if (action == "remove") {

        std::string name(get_param(&params, 2));
        auto query = new SQLite3QueryBuilder("asset");
        query->where({std::make_pair("owner", username), std::make_pair("name", name)});

        Asset asset(query);
        asset.del();

        return 0;
    } else if (action == "modify") {

        std::string name(get_param(&params, 2));
        std::string opt(get_param(&params, 3));
        std::string val(get_param(&params, 4));

        auto query = new SQLite3QueryBuilder("asset");
        query->where({std::make_pair("owner", username), std::make_pair("name", name)});
        Asset asset(query);
        bool success;

        if (opt == "--value")
            success = asset.update({std::make_pair("value", val)});
        else if (opt == "--name")
            success = asset.update({std::make_pair("type", val)});
        else
            return error_msg("Invalid option.");

        if (success)
            return 0;
        else
            return error_msg("Could not update asset.");
    } else
        return error_msg("Invalid command.");
}


int new_income(const std::string& username)
{
    std::string type;
    std::string hours;
    std::string amount;
    std::string pay_frequency;
    std::string to_acct;
    bool valid = false;

    while (!valid) {
        std::cout << "Enter the type of income (salary or hourly): " << std::endl;
        std::cin >> type;

        if (type == "salary" || type == "hourly") {
            valid = true;
        } else {
            std::cout << "Income must be salary or hourly." << std::endl;
        }
    }

    if (type == "hourly") {
        valid = false;
        while (!valid) {
            std::cout << "Enter the number of hours per week: " << std::endl;
            std::cin >> hours;

            try {
                std::stoi(hours);
                valid = true;
            } catch (std::invalid_argument& err) {
                std::cout << "Hours must be an integer number. " << std::endl;
            }
        }
    }

    if (type == "hourly") {
        valid = false;
        while (!valid) {
            std::cout << "Enter the pay per hour: " << std::endl;
            std::cin >> amount;

            try {
                std::stod(amount);
                valid = true;
            } catch (std::invalid_argument &err) {
                std::cout << "Amount must be a decimal number." << std::endl;
            }
        }
    } else if (type == "salary") {
        valid = false;
        while (!valid) {
            std::cout << "Enter the yearly salary: " << std::endl;
            std::cin >> amount;

            try {
                std::stod(amount);
                valid = true;
            } catch (std::invalid_argument &err) {
                std::cout << "Amount must be a decimal number." << std::endl;
            }
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the pay frequency, in terms of paychecks per year (i.e. 52 for weekly, 12 for monthly, etc): " << std::endl;
        std::cin >> pay_frequency;

        try {
            std::stoi(pay_frequency);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Frequency must be an integer number." << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the account number to which the income will be deposited: " << std::endl;
        std::cin >> to_acct;

        auto query = new SQLite3QueryBuilder("account");
        query->where({std::make_pair("owner", username), std::make_pair("acct_num", to_acct)});
        Account to(query);
        if (to.get()->row_count() == 1)
            valid = true;
        else
            std::cout << "That account number doesn't correspond to a valid account." << std::endl;
    }

    if (Income::create({username, type, hours, amount, pay_frequency, to_acct}))
        return 0;
    else
        return error_msg("Could not create income.");
}

int Controller::income(const std::string& username, const std::vector<std::string> params)
{
    std::string action = get_param(&params, 1);

    if (action == "view") {

        std::string id = get_param(&params, 2);
        auto query = new SQLite3QueryBuilder("income");

        if (id.empty()) {
            query
            ->select({})
            ->where(std::make_pair("owner", username));
        } else {
            query
            ->select({})
            ->where({std::make_pair("owner", username), std::make_pair("name", id)});
        }

        auto res = SQLite3Instance::getInstance()->query(query);
        TableView::view(res);

        return 0;

    } else if (action == "add") {

        return new_income(username);

    } else if (action == "remove") {

        std::string id = get_param(&params, 2);

        if (id.empty())
            return error_msg("An income name must be provided in order to remove an income. Use the command 'income view' to view the information about all of your incomes.");

        auto query = new SQLite3QueryBuilder("income");
        query->where({std::make_pair("owner", username), std::make_pair("alias", id)});

        Income income(query);
        income.del();

        return 0;

    } else if (action == "modify") {

        std::string id(get_param(&params, 2));
        std::string opt(get_param(&params, 3));
        std::string val(get_param(&params, 4));
        std::string col;

        if (opt == "--type")
            col = "type";
        else if (opt == "--hours")
            col = "hours";
        else if (opt == "--amount")
            col = "amount";
        else if (opt == "--pay_frequency")
            col = "pay_freq";
        else if (opt == "--to_acct")
            col = "to_acct";
        else
            return error_msg("Can't update income with the given arguments.");

        auto query = new SQLite3QueryBuilder("income");
        query->where({std::make_pair("owner", username), std::make_pair("alias", id)});

        Income income(query);

        if (income.get()) {
            if (income.update({std::make_pair(col, val)}))
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
    std::string principal;
    std::string interest;
    std::string start_date;
    std::string maturity_date;
    std::string type;
    std::string from_acct;
    bool valid = false;

    while (!valid) {
        std::cout << "Enter the principal amount: " << std::endl;
        std::cin >> principal;

        try {
            std::stod(principal);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Principal must be a decimal number. " << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the interest rate: " << std::endl;
        std::cin >> interest;

        try {
            std::stod(interest);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Interest must be a decimal number. " << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the debt start date in YYYY-MM-DD format: " << std::endl;
        std::cin >> start_date;

        try {
            if (std::regex_match(start_date, std::regex("^[0-9]{4}-[0-1][0-9]-[0-3][0-9]$")))
                valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "The date must be in YYYY-MM-DD format. " << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the debt maturity date in YYYY-MM-DD format: " << std::endl;
        std::cin >> maturity_date;

        try {
            if (std::regex_match(start_date, std::regex("^[0-9]{4}-[0-1][0-9]-[0-3][0-9]$")))
                valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "The date must be in YYYY-MM-DD format. " << std::endl;
        }
    }

    std::cout << "Enter the debt type (i.e. mortgage, car, other loan): " << std::endl;
    std::getline(std::cin, type);

    valid = false;
    while (!valid) {
        std::cout << "Enter the account number from which the debt will be paid: " << std::endl;
        std::cin >> from_acct;

        auto query = new SQLite3QueryBuilder("account");
        query->where({std::make_pair("owner", username), std::make_pair("acct_num", from_acct)});
        Account to(query);
        if (to.get()->row_count() == 1)
            valid = true;
        else
            std::cout << "That account number doesn't correspond to a valid account." << std::endl;
    }

    if (Debt::create({username, principal, interest, start_date, maturity_date, type, from_acct}))
        return 0;
    else
        return error_msg("Could not create debt.");
}

int Controller::debt(const std::string& username, const std::vector<std::string> params)
{
    std::string action(get_param(&params, 1));

    if (action == "view") {

        std::string id(get_param(&params, 2));
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

        std::string id(get_param(&params, 2));
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

        std::string id(get_param(&params, 2));
        std::string opt(get_param(&params, 3));
        std::string val(get_param(&params, 4));

        if (opt != "--from_acct")
            return error_msg("Could not update debt with the given arguments.");

        auto query = new SQLite3QueryBuilder("debt");

        if (id.empty())
            return error_msg("A specific debt must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Debt debt(query);

        if (debt.get()) {
            if (debt.update({std::make_pair("from_acct", val)}))
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
    std::string type;
    std::string ticker;
    std::string buy_price;
    std::string num_shares;
    std::string buy_date;
    std::string acct;
    bool valid = false;

    std::cout << "Enter the type of security: " << std::endl;
    std::getline(std::cin, type);

    std::cout << "Enter the security's ticker symbol: " << std::endl;
    std::cin >> ticker;

    while (!valid) {
        std::cout << "Enter the buy price (per share): " << std::endl;
        std::cin >> buy_price;

        try {
            std::stod(buy_price);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Buy price must be a valid decimal number. " << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the number of shares purchased: " << std::endl;
        std::cin >> num_shares;

        try {
            std::stoi(num_shares);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Number of shares must be a valid decimal number. " << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the buy date in YYYY-MM-DD format: " << std::endl;
        std::cin >> buy_date;

        try {
            if (std::regex_match(buy_date, std::regex("^[0-9]{4}-[0-1][0-9]-[0-3][0-9]$")))
                valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "The date must be in YYYY-MM-DD format. " << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the account number with which this security was purchased." << std::endl;
        std::cin >> acct;

        auto query = new SQLite3QueryBuilder("account");
        query->where({std::make_pair("owner", username), std::make_pair("acct_num", acct)});
        Account to(query);
        if (to.get()->row_count() == 1)
            valid = true;
        else
            std::cout << "That account number doesn't correspond to a valid account." << std::endl;
    }

    if (Investment::create({username, type, ticker, buy_price, num_shares, buy_date, acct}))
        return 0;
    else
        return error_msg("Could not create investment.");

    return 0;
}

int Controller::investments(const std::string& username, const std::vector<std::string> params)
{
    std::string action(get_param(&params, 1));

    if (action == "view") {

        std::string id(get_param(&params, 2));
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

        std::string id(get_param(&params, 2));
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

        return error_msg("Not allowed.");

    } else
        return error_msg("Invalid command.");
}


int new_budget(const std::string& username)
{
    std::string user;
    std::string goal_account;
    std::string goal_amount;
    std::string description;
    bool valid = false;

    while (!valid) {
        std::cout << "Enter the username of the user who this budget is for: " << std::endl;
        std::cin >> user;

        std::string sql = "SELECT username FROM auth_user WHERE username = ?1 AND family_id = ( SELECT family_id FROM family WHERE owner = ?2 );";
        auto params = new std::vector<std::string>{user, username};
        auto res = SQLite3Instance::getInstance()->query(sql, params);

        if (res->row_count() == 1) {
            valid = true;
        } else {
            std::cout << "Cannot create a budget for that user." << std::endl;
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the account number which this budget is associated with: " << std::endl;
        std::cin >> goal_account;

        auto query = new SQLite3QueryBuilder("account");
        query->where({std::make_pair("owner", user), std::make_pair("acct_num", goal_account)});
        Account to(query);
        if (to.get()->row_count() == 1)
            valid = true;
        else
            std::cout << "That account number doesn't correspond to a valid account." << std::endl;
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter the goal amount for this budget (either expenditure or savings): " << std::endl;
        std::cin >> goal_amount;

        try {
            std::stod(goal_amount);
            valid = true;
        } catch (std::invalid_argument &err) {
            std::cout << "Goal amount must be a decimal number" << std::endl;
        }
    }

    std::cout << "Enter a description of this budget: " << std::endl;
    std::getline(std::cin, description);

    if (Budget::create({user, goal_account, goal_amount, description}))
        return 0;
    else
        return error_msg("Could not create budget.");

    return 0;
}

int Controller::budget(const std::string& username, const std::vector<std::string> params)
{
    std::string action(get_param(&params, 1));

    if (action == "view") {

        std::string id(get_param(&params, 2));
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

        std::string id(get_param(&params, 2));
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

        std::string id(get_param(&params, 2));
        std::string opt(get_param(&params, 3));
        std::string val(get_param(&params, 4));
        std::string col;

        if (opt == "--goal_amount")
            col = "goal_amount";
        else if (opt == "--description")
            col = "description";

        auto query = new SQLite3QueryBuilder("budget");

        if (id.empty())
            return error_msg("A specific budget must be specified.");

        query->where({std::make_pair("owner", username), std::make_pair("id", id)});

        Budget budget(query);

        if (budget.get()) {
            if (budget.update({std::make_pair(col, val)}))
                return 0;
            else
                return error_msg("Could not update the budget.");
        } else
            return error_msg("Could not find the budget.");
    } else
        return error_msg("Invalid command.");
}

int Controller::overview(const std::string &username, const std::vector<std::string> params)
{
    Controller::account(username, {"account", "view"});
    Controller::assets(username, {"assets", "view"});
    Controller::income(username, {"income", "view"});
    Controller::debt(username, {"debt", "view"});
    Controller::budget(username, {"budget", "view"});
    return 0;
}


int Controller::family(const std::string &username, const std::vector<std::string> params)
{
    std::string action(get_param(&params, 1));

    if (action == "view") {

        try {

            std::string get_id_sql = "SELECT family_id FROM family WHERE owner = ?1;";
            auto param_id = new std::vector<std::string>{username};
            auto id_row = SQLite3Instance::getInstance()->query(get_id_sql, param_id);
            std::string id = id_row->get_row(0)->at(0);

            std::string get_usernames_sql = "SELECT username FROM auth_user WHERE family_id = ?1;";
            auto param_usernames = new std::vector<std::string>{id};
            auto usernames = SQLite3Instance::getInstance()->query(get_usernames_sql, param_usernames);

            TableView::view(usernames);
            return 0;
        } catch (std::runtime_error &err) {
            return error_msg("Could not retrieve usernames.");
        }

    } else if (action == "create") {

        std::string sql = "INSERT INTO family (owner) VALUES ( ?1 );";
        auto args = new std::vector<std::string>{username};
        if (SQLite3Instance::getInstance()->query(sql, args) != nullptr)
            return 0;
        else
            return error_msg("Could not create family.");

    } else if (action == "add") {

        std::string addUser = get_param(&params, 2);
        if (!addUser.empty()) {

            try {
                std::string get_id_sql = "SELECT family_id FROM family WHERE owner = ?1;";
                auto param_id = new std::vector<std::string>{username};
                auto id_row = SQLite3Instance::getInstance()->query(get_id_sql, param_id);
                std::string id = id_row->get_row(0)->at(0);

                std::string add_sql = "UPDATE auth_user SET family_id = ?1 WHERE username = ?2;";
                auto param_add = new std::vector<std::string>{id, addUser};

                if (SQLite3Instance::getInstance()->query(add_sql, param_add) != nullptr)
                    return 0;
                else
                    return error_msg("Could not add user to family.");
            } catch (std::runtime_error &err) {
                return error_msg("Could not add user to family.");
            }
        } else
            return error_msg("A username must be provided.");


    } else if (action == "remove") {

        std::string removeUser = get_param(&params, 2);
        if (!removeUser.empty()) {

            try {
                std::string get_id_sql = "SELECT family_id FROM family WHERE owner = ?1;";
                auto param_id = new std::vector<std::string>{username};
                auto id_row = SQLite3Instance::getInstance()->query(get_id_sql, param_id);
                std::string id = id_row->get_row(0)->at(0);

                std::string add_sql = "UPDATE auth_user SET family_id = '' WHERE username = ?1;";
                auto param_add = new std::vector<std::string>{removeUser};

                if (SQLite3Instance::getInstance()->query(add_sql, param_add) != nullptr)
                    return 0;
                else
                    return error_msg("Could not remove user from family.");
            } catch (std::runtime_error &err) {
                return error_msg("Could not remove user from family..");
            }
        } else
            return error_msg("A username must be provided.");

    } else
        return error_msg("Invalid arguments.");
}
