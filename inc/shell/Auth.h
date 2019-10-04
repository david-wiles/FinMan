#ifndef FINMAN_AUTH_H
#define FINMAN_AUTH_H

#include <db/SQLite3.h>


class Auth
{
public:
    explicit Auth(SQLite3* database) : _database(database), _username(nullptr) {};

    /**
     * Prompt the user to either log in or create an account, and save the resulting username
     */
    void authenticate();

    /**
     * Return the authenticated user's username as a string
     *
     * @return  String username
     */
    std::string get_username();

    ~Auth() = default;

private:
    // Currently authenticated user's username
    char* _username;
    // Pointer to the database instance
    SQLite3* _database;

    /**
     * Handle existing user login
     */
    void existing_user();

    /**
     * Handle new user creation and login
     */
    void new_user();

    /**
     * Set the username of the currently authenticated user
     *
     * @param name  Username to set
     */
    void set_username(char* name);
};

#endif //FINMAN_AUTH_H
