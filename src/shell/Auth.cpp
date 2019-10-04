#include <openssl/md5.h>
#include <string>
#include <iostream>

#include "shell/Auth.h"


static int create_user(void *new_username, int argc, char **argv, char **azColName);
static int auth_user(void *expected_username, int col_count, char **col_data, char **col_name);


void Auth::authenticate(sqlite3* db)
{
    std::cout << "Log in? (y/n), press n to create a new user.";

    if (getchar() == 'y') {
        existing_user(db);
    } else {
        new_user(db);
    }
}


void Auth::existing_user(sqlite3* db)
{
    while (true) {
        char *zErrMsg = nullptr;
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

        sqlite3_exec(db, sql, auth_user, (void *) user_string.c_str(), &zErrMsg);
        if (Auth::username != nullptr)
            break;
    }
}

void Auth::new_user(sqlite3* db)
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

            sqlite3_exec(db, sql, create_user, (void*) user_string.c_str(), &zErrMsg);
            if (Auth::username != nullptr)
                break;
        } else {
            std::cout << "Passwords do not match." << std::endl;
        }
    }
}

void Auth::set_username(char* name)
{
    Auth::username = name;
}

static int create_user(void *new_username, int argc, char **argv, char **azColName)
{

}

static int auth_user(void *expected_username, int col_count, char **col_data, char **col_name)
{
    char* username = static_cast<char *>(expected_username);
    char expected = '1';
    if (col_data[0] == &expected && col_data[1] == username) {
        Auth::set_username(username);
    }

    return 0;
}