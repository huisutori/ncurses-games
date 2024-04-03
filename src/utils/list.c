/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#include <stdlib.h>
#include "list.h"

typedef struct List {
    ListNode *head;
} List;

List *List_new(void)
{
    List *list = malloc(sizeof(List));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    
    return list;
}

void List_delete(List *list)
{
    if (list) {
        free(list);
    }
}

int List_add(List *list, ListNode *node)
{
    if (!list) {
        return -1;
    }
    node->next = list->head;
    list->head = node;
    
    return 0;
}

void List_remove(List *list, ListNode *node)
{
    if (!list) {
        return;
    }
    ListNode *prev_node = NULL;
    ListNode *curr_node = list->head;
    
    while (curr_node) {
        if (curr_node == node) {
            if (prev_node) {
                prev_node->next = curr_node->next;
            } else {
                list->head = curr_node->next;
            }
            break;
        }
        prev_node = curr_node;
        curr_node = curr_node->next;
    }
}

ListNode *List_get_head(List *list)
{
    return list ? list->head : NULL;
}

ListNode *List_get_next(ListNode *node)
{
    return node ? node->next : NULL;
}

void List_iterate(List *list, void (*iter)(ListNode *node))
{
    if (!list) {
        return;
    }
    ListNode *curr_node = list->head;
    while (curr_node) {
        iter(curr_node);
        curr_node = curr_node->next;
    }
}