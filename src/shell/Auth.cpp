#include <openssl/sha.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

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

void Auth::set_username(const char* name)
{
    this->_username = const_cast<char *>(name);
}

std::string Auth::hash_password(std::string password)
{
    const char *pass_arr = password.c_str();
    unsigned char sha1[SHA_DIGEST_LENGTH + 1];
    SHA1((unsigned char *) pass_arr, password.size(), sha1);

    // Convert to string (credit to Nayfe on StackOverflow ... https://stackoverflow.com/questions/918676/generate-sha-hash-in-c-using-openssl-library)
    std::stringstream shastr;
    shastr << std::hex << std::setfill('0');
    for (const auto& byte: sha1)
        shastr << std::setw(2) << (int) byte;

    return shastr.str();
}


bool Auth::query_for_user(std::string username, std::string password)
{
    char sql[] = "SELECT COUNT(*), username FROM auth_user WHERE username = ?1 AND pass_hash = ?2;";

    std::string hash = hash_password(password);

    std::vector<std::string> params{username, hash};
    DB_Result* res = SQLite3::getInstance()->execute(std::string(sql), &params);

    std::vector<std::string> expected{"1", params.at(0)};

    if (*res->get_row(0) == expected) {
        set_username(username.c_str());
        return true;
    }

    return false;
}

void Auth::existing_user()
{
    while (true) {
        std::cout << "Username: ";
        std::string user_string;
        std::cin >> user_string;

        std::cout << "Password: ";
        std::string password;
        std::cin >> password;

        if (query_for_user(user_string, password))
            break;
    }
}

void Auth::new_user()
{
    while (true) {
        char sql[] = "INSERT INTO auth_user (username, pass_hash) VALUES (?1, ?2);";

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
            std::string hash = hash_password(password);

            std::vector<std::string> params{user_string, hash};
            SQLite3::getInstance()->execute(std::string(sql), &params);

            if (query_for_user(user_string, password))
                break;

        } else {
            std::cout << "Passwords do not match." << std::endl;
        }
    }
}
