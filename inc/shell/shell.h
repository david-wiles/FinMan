#ifndef FINMAN_SHELL_H
#define FINMAN_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_DELIMS " \t\r\n\a"
#define NUM_COMMANDS 5

typedef struct Linked_Node Linked_Node;

struct Linked_Node {
    char** args;
    struct Linked_Node* prev;
    struct Linked_Node* next;
};

char* commands_str[NUM_COMMANDS];
int (*commands[NUM_COMMANDS]) (char**);

void shell_loop();
char* shell_read_line();
char** shell_get_args(char* line);
int shell_execute(char** args, Linked_Node* history);

/**
 * Add a node to a doubly linked list
 *
 * @param list Pointer to the list's first element
 * @param node The node to add to the list
 */
inline void add_node(Linked_Node* list, Linked_Node* node)
{
    list->next = node;
    node->prev = list;
}

/**
 * Free all memory used in a linked list
 *
 * @param this Pointer to the first element in the linked list
 */
inline void free_nodes(Linked_Node* this)
{
    Linked_Node* prev = this->prev;
    while (prev != NULL) {
        free(this);
        this = prev;
        prev = this->prev;
    }
    free(this);
}

#endif //FINMAN_SHELL_H
