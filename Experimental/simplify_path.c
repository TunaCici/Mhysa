#include <stdlib.h>
#include <string.h>

typedef struct my_node {
    char *name;
    size_t len;
    struct my_node *next;
    struct my_node *prev;
} my_node;

size_t conslen(char *str)
{   
        size_t len = 0;
        while(str[len] != '\0' && str[len] == str[0]) {
                len++;
        }

        return len;
}

size_t namelen(char *str)
{
        size_t len = 0;
        while (str[len] != '\0' && str[len] != '/') {
                len++;
        }

        return len;
}

size_t dotlen(char *str)
{
        size_t len = 0;
        while (str[len] != '\0' && str[len] != '/') {
                if (str[len] != '.') {
                        return 0;
                }

                len++;
        }

        return len;
}

void append_node(my_node *tail, my_node *new_node)
{
        if (!tail || !new_node) {
                return;
        }

        tail->next = new_node;
        new_node->prev = tail;
}

void remove_node(my_node **head, my_node *target)
{
        if (!target) {
                return;
        }

        // prev
        if (target->prev) {
                target->prev->next = target->next;
        }

        // next
        if(target->next) {
                target->next->prev = target->prev;
        }

        if (*head == target) {
                *head = target->next;
        }
}

char* simplifyPath(char* path)
{
        if (!path) {
                return NULL;
        }

        my_node *head = NULL;
        my_node *tail = NULL;

        size_t path_len = strlen(path);

        for (int i = 0; i < path_len; i++) {
                if (path[i] == '/') {
                        i += (conslen(&path[i]) - 1);
                } else {
                        my_node *new_node = (my_node*) malloc(sizeof(my_node));
                        new_node->name = &path[i];
                        new_node->len = namelen(&path[i]);
                        new_node->next = NULL;
                        new_node->prev = NULL;

                        if (tail) {
                                append_node(tail, new_node);
                        } else {
                                head = new_node;
                        }

                        i += (new_node->len - 1);
                        tail = new_node;
                }
        }

        // handle '.' and '..'
        my_node *iter = head;
        while (iter) {
                if (iter->name[0] == '.' && dotlen(iter->name) == 1) {
                        my_node *tmp = iter->next;

                        remove_node(&head, iter);

                        iter = tmp;
                } else if (iter->name[0] == '.' && dotlen(iter->name) == 2) {
                        my_node *tmp = iter->next;
                        my_node *prev = iter->prev;

                        remove_node(&head, prev);
                        remove_node(&head, iter);

                        iter = tmp;
                } else {
                        iter = iter->next;
                }
        }

        // reconstruct the path
        char *ret = NULL;
        size_t ret_len = 1;

        iter = head;
        while (iter) {
                ret_len += (iter->len + 1);
                iter = iter->next;
        }

        ret = (char*) malloc((ret_len + 1) * sizeof(char));

        // iter was empty
        if (ret_len == 1) {
                ret[0] = '/';
                ret[1] = '\0';
                return ret;
        }

        iter = head;
        size_t ret_idx = 0;

        while (iter) {
                ret[ret_idx++] = '/';
                strncpy(&ret[ret_idx], iter->name, iter->len);
                ret_idx += iter->len;
                iter = iter->next;
        }
        ret[ret_idx] = '\0';

        return ret;
}
