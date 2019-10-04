#include <openssl/md5.h>
#include <string>
#include <iostream>

#include "shell/Auth.h"


void Auth::authenticate()
{
    std::cout << "Log in? (y/n), press n to create a new user.";

    if (getchar() == 'y') {
        existing_user();
    } else {
        new_user();
    }
}

std::string Auth::get_username()
{
    return std::string(this->_username);
}

void Auth::existing_user()
{
    while (true) {
        char sql[] = "SELECT COUNT(*), username FROM auth_user WHERE username = ? AND password = ?;";

        std::cout << "Username: ";
        std::string user_string;
        std::cin >> user_string;

        std::cout << "Password: ";
        std::string password;
        std::cin >> password;

        const char *pass_arr = password.c_str();
        unsigned char md5[MD5_DIGEST_LENGTH];
        MD5((unsigned char *) pass_arr, password.size(), md5);

        std::vector<std::string> params({user_string, std::string((char*)md5)});
        this->_database->execute(sql, &params);
        if (this->_username != nullptr)
            break;
    }
}

void Auth::new_user()
{
    while (true) {
        char *zErrMsg = nullptr;
        char sql[] = "INSERT INTO auth_user (username, password) VALUES (?, ?);";

        std::cout << "Username: ";
        std::string user_string;
        std::cin >> user_string;

        std::cout << "Password: ";
        std::string password;
        std::cin >> password;

        std::cout << "Password: ";
        std::string password2;
        std::cin >> password2;

        if (password == password2) {
            const char *pass_arr = password.c_str();
            unsigned char md5[MD5_DIGEST_LENGTH];
            MD5((unsigned char *) pass_arr, password.size(), md5);

            if (this->_username != nullptr)
                break;
        } else {
            std::cout << "Passwords do not match." << std::endl;
        }
    }
}

void Auth::set_username(char* name)
{
    this->_username = name;
}
