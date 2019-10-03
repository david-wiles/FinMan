#include <shell/shell.h>


void loop(char* username)
{
    int status;

    do {
        printf("FinMan > ");
        status = execute(username, get_args());
    } while (status);

}

char** get_args()
{
    char* input = NULL;
    int buffer_size = 64;
    int position = 0;
    char* token;
    char** tokens = malloc(buffer_size * sizeof(char*));

    // Read a line of input from console
    input = readline(input);
    while (strlen(input) == 0)
        input = readline(input);

    if (!tokens) {
        fprintf(stderr, "Shell could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    // Split input into string array
    token = strtok(input, LINE_DELIMS);

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

int execute(char* username, char** args)
{
    int status = 0;
    if (args[0] == NULL)
        status = 1;

    for (int i = 0; i < NUM_COMMANDS; ++i) {
        if (strcmp(args[0], commands_str[i]) == 0) {
            status = (*commands[i])(username, args);
            break;
        }
    }

    return status;
}
