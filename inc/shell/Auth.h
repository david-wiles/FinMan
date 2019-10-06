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
    void set_username(const char* name);

    /**
     * Create a hashed string based on a char string password
     *
     * @param password  Password to hash as a char array
     * @return          String hashed password
     */
    static std::string hash_password(std::string password);

    /**
     * Query for a username and password match.  If a match is found, the instance's username is set to the queried
     * username.
     *
     * @param username  Username as a string
     * @param pass_hash Password as a string, to be hashed in the function
     * @return          Boolean indicating if the user was found.
     */
    bool query_for_user(std::string username, std::string password);
};

#endif //FINMAN_AUTH_H
