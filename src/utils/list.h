/*
 *  Author: Huisu Ju <kng0258@naver.com>
 *  License: MIT
 */
#pragma once

typedef struct List List;

typedef struct ListNode {
    struct ListNode *next;
} ListNode;

#define LIST_NODE   \
    ListNode list_entry;

List *List_new(void);
void List_delete(List *list);

int List_add(List *list, ListNode *node);
void List_remove(List *list, ListNode *node);

ListNode *List_get_head(List *list);
ListNode *List_get_next(ListNode *node);

void List_iterate(List *list, void (*iter)(ListNode *node));