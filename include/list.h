#ifndef LIST_H
#define LIST_H

struct ListNode {
    char* file_path;
    struct ListNode* next;
};

struct List {
    struct ListNode* head;
    struct ListNode* tail;
};

void init_list(struct List* list);
void destroy_list(struct List* list);
void append_list(struct List* list, char* file_path);
void concat_lists(struct List* list_dst, struct List* list_src);

#endif