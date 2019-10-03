#ifndef FINMAN_SHELL_H
#define FINMAN_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sqlite3.h>

#define NUM_COMMANDS 5
#define LINE_DELIMS " \t\r\n\a"

char* commands_str[NUM_COMMANDS];
int (*commands[NUM_COMMANDS]) (char*, char**);


/**
 * Shell main loop
 *
 * @param username The username to pass as an argument to commands requiring authentication
 */
void loop(char* username);

/**
 * Read a line of input from console and split into string tokens to pass to execute
 *
 * @return Array of arguments
 */
char** get_args();

/**
 * Execute a command passed
 *
 * @param username The username to pass as an argument to commands requiring authentication
 * @param args Array of arguments
 * @return Return status of the executed command
 */
int execute(char* username, char** args);

#endif //FINMAN_SHELL_H
