#include <iostream>
#include <model/QueryResult.h>
#include <model/SQLite3QueryBuilder.h>
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

void error_msg(std::string err)
{
    std::cout << err << std::endl;
}

int new_account(const std::string username)
{
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

int Controller::account(std::string username, const std::vector<std::string>* params)
{
    std::string action(get_param(params, 1));
    if (action == "view") {

        std::string acct(get_param(params, 2));
        QueryResult* res;

        if (!acct.empty()) {

            auto* cols = new std::vector<std::string>();
            auto* where = new std::vector<std::pair<std::string,std::string>>({
                std::make_pair("owner", username),
                std::make_pair("acct_num", acct)
            });
            res = SQLite3QueryBuilder::getInstance()->findRows(std::string("account"), cols, where);
            delete(cols);
            delete(where);

        } else {

            auto* cols = new std::vector<std::string>();
            auto* where = new std::vector<std::pair<std::string,std::string>>({
                std::make_pair("owner", username),
            });
            res = SQLite3QueryBuilder::getInstance()->findRows(std::string("account"), cols, where);
            delete(cols);
            delete(where);

        }

        TableView results(res);
        results.print();
        return 0;

    } else if (action == "create") {

        return new_account(username);

    } else if (action == "modify") {

        std::string acct(get_param(params, 3));

        if (!acct.empty()) {

            std::string opt(get_param(params, 4));
            std::string col(get_param(params, 5));

            if (!opt.empty()) {

                std::string val;
                if (opt == "-u")
                    val = get_param(params, 6));
                else if (opt != "-r")
                    ;
                    // TODO return errors

                // TODO determine cols to allow to update and create if/else statements
                auto* set = new std::vector<std::pair<std::string,std::string>>({
                    std::make_pair(col, val)
                });
                auto* where = new std::vector<std::pair<std::string,std::string>>({
                    std::make_pair("owner", username),
                    std::make_pair("acct_num", acct)
                });
                bool res = SQLite3QueryBuilder::getInstance()->updateRow("account", set, where);

                if (res)
                    return 0;
                else
                    ;
                // TODO return errors
            }

        }

    }

    std::cout << "Could not perform the requested action with given arguments." << std::endl;
    return 0;
}

int assets(std::string username, const std::vector<std::string>* params);
int income(std::string username, const std::vector<std::string>* params);
int investments(std::string username, const std::vector<std::string>* params);
int budget(std::string username, const std::vector<std::string>* params);