#include "../../inc/shell/shell.h"

/**
 * Main loop for the shell.
 *
 * @return The return value of the shell to be passed to the program's main
 */
void shell_loop()
{
    char *line;
    char **args;
    int status;
    Linked_Node history = {};

    do {
        printf("FinMan> ");
        line = shell_read_line();
        args = shell_get_args(line);
        status = shell_execute(args, &history);

    } while (status);
}

/**
 * Read a line from stdin
 *
 * @return pointer to the line
 */
char* shell_read_line()
{
    char* line = NULL;
    size_t buffer_size = 0;
    getline(&line, &buffer_size, stdin);
    return line;
}

/**
 * Parse the line into a string array of args
 *
 * @param line The line, read from stdin
 * @return An array of arguments
 */
char** shell_get_args(char* line)
{
    int buffer_size = 64;
    int position = 0;
    char* token;
    char** tokens = malloc(buffer_size * sizeof(char*));

    if (!tokens) {
        fprintf(stderr, "Shell could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LINE_DELIMS);
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= buffer_size) {
            buffer_size += buffer_size;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "Shell could not reallocate tokens\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LINE_DELIMS);
    }
    tokens[position] = NULL;
    return tokens;
}

int shell_execute(char** args, Linked_Node* history)
{
    int status = 0;
    if (args[0] == NULL)
        status = 1;

    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (strcmp(args[0], commands_str[i]) == 0) {
            Linked_Node* cmd = malloc(sizeof(Linked_Node));
            cmd->args = args;
            add_node(history, cmd);

            status = (*commands[i])(args);
            break;
        }
    }

    free(args);
    return status;
}