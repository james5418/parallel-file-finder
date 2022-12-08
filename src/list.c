#include "list.h"

#include <stdio.h>

void init_list(struct List* list) { list->head = list->tail = NULL; }

void destroy_list(struct List* list) {
    struct ListNode* curr = list->head;
    struct ListNode* next;

    while (curr) {
        next = curr->next;
        free(curr->file_path);
        free(curr);
        curr = next;
    }
}

void append_list(struct List* list, char* file_path) {
    struct ListNode* new_node = (struct ListNode*)malloc(sizeof(struct ListNode));

    new_node->file_path = file_path;
    new_node->next = NULL;

    if (list->head) {
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        list->head = new_node;
        list->tail = new_node;
    }
}

void concat_lists(struct List* list_dst, struct List* list_src) {
    if (!list_src->head) return;

    if (list_dst->head)
        list_dst->tail->next = list_src->head;
    else
        list_dst->head = list_src->head;

    list_dst->tail = list_src->tail;
}
