#include <iostream>
#include <model/QueryResult.h>
#include <model/SQLite3DB.h>
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

int Controller::account(std::string username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));
    if (action == "view") {

        std::string acct(get_param(params, 2));
        QueryResult* res;

        if (!acct.empty()) {
            std::string sql = "SELECT * FROM account WHERE owner = ?1 AND account_num = ?2;";
            auto* where = new std::vector<std::string>({username, acct});
            res = SQLite3DB::getInstance()->query(sql, where);
            delete(where);
        } else {
            std::string sql = "SELECT * FROM account WHERE owner = ?1;";
            auto* where = new std::vector<std::string>({username});
            res = SQLite3DB::getInstance()->query(sql, where);
            delete(where);
        }

        TableView results(res);
        results.print();
        return 0;
    } else if (action == "create") {

    } else if (action == "modify") {

    }

    std::cout << "Could not perform the requested action with given arguments." << std::endl;
    return 0;
}

int assets(std::string username, const std::vector<std::string>* params);
int income(std::string username, const std::vector<std::string>* params);
int investments(std::string username, const std::vector<std::string>* params);
int budget(std::string username, const std::vector<std::string>* params);