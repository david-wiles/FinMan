#ifndef FINMAN_AUTH_H
#define FINMAN_AUTH_H

#include <db/SQLite3Instance.h>

/**
 * Auth should handle authentication across the site
 */
class Auth
{
public:
    Auth() : _username() {};

    /**
     * Create an auth object from a given username and password. Throws a runtime error if the user cannot be found
     * (should only be used with system users)
     *
     * @param username
     * @param password
     */
    Auth(std::string username, std::string password);

    /**
     * Prompt the user to either log in or create an account and save the resulting username
     */
    static std::string authenticate();

    /**
     * Return the authenticated user's username as a string
     *
     * @return  String username
     */
    inline std::string get_username() const { return this->_username; };

    ~Auth() = default;

private:
    // Currently authenticated user's username
    std::string _username;

    /**
     * Handle existing user login
     */
    static std::string existing_user();

    /**
     * Handle new user creation and login
     */
    static std::string new_user();

    /**
     * Query for a username and password match.  If a match is found, the instance's username is set to the queried
     * username.
     *
     * @param username  Username as a string
     * @param pass_hash Password as a string, to be hashed in the function
     * @return          Boolean indicating if the user was found.
     */
    static bool query_for_user(std::string username, std::string password);

};

#endif //FINMAN_AUTH_H
