#include <openssl/sha.h>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <db/SQLite3QueryBuilder.h>

#include "shell/Auth.h"


Auth::Auth(const std::string& username, const std::string& password) : _username()
{
    if (query_for_user(username, password))
        this->_username = username;
    else
        throw std::runtime_error("Could not authenticate user.");
}


std::string Auth::authenticate()
{
    std::cout << "Press any key to log in. ('n' to create a new user).";
    std::string username;

    if (getchar() == 'n') {
        username = new_user();
    } else {
        username = existing_user();
    }

    return username;
}

/**
 * Create a password hash based on a plain text password
 *
 * @param password  Password to hash as a char array
 * @return          hashed password as a string
 */
std::string hash_password(const std::string& password)
{
    // Create hex hash
    const char *pass_arr = password.c_str();
    unsigned char sha1[SHA_DIGEST_LENGTH + 1];
    SHA1((unsigned char *) pass_arr, password.size(), sha1);

    // Convert hashed bytes to char string
    std::stringstream shastr;
    shastr << std::hex << std::setfill('0');
    for (const auto& byte: sha1)
        shastr << std::setw(2) << (int) byte;

    return shastr.str();
}


bool Auth::query_for_user(std::string username, const std::string& password)
{
    std::string sql = "SELECT COUNT(*), username FROM auth_user WHERE username = ?1 AND pass_hash = ?2;";

    std::string hash = hash_password(password);

    std::vector<std::string> params{std::move(username), hash};
    QueryResult* res = SQLite3Instance::getInstance()->query(sql, &params);

    std::vector<std::string> expected{"1", params.at(0)};

    bool found = *res->get_row(0) == expected;

    return found;
}

std::string Auth::existing_user()
{
    while (true) {
        std::cout << "Username: ";
        std::string username;
        std::cin >> username;

        std::cout << "Password: ";
        std::string password;
        std::cin >> password;

        if (query_for_user(username, password))
            return username;
    }
}

std::string Auth::new_user()
{
    while (true) {
        std::string sql = "INSERT INTO auth_user (username, pass_hash) VALUES (?1, ?2);";

        std::cout << "Username: ";
        std::string username;
        std::cin >> username;

        std::cout << "Password: ";
        std::string password;
        std::cin >> password;

        std::cout << "Password: ";
        std::string password2;
        std::cin >> password2;


        if (password == password2) {

            auto query = new SQLite3QueryBuilder("auth_user");
            query
            ->insert({"username", "pass_hash"})
            ->values({{username, hash_password(password)}});

            SQLite3Instance::getInstance()->query(query);

            if (query_for_user(username, password))
                return username;
            else
                throw std::runtime_error("Could not create user.");

        } else {
            std::cout << "Passwords do not match." << std::endl;
        }
    }
}
