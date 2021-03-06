#include "list.h"

#include <stdio.h>
#include <stdlib.h>

// ****************************
// Utility
// ****************************

// @size [bytes]
void* allocate(int size)
{
    void* retval = malloc(size);

    if (retval == NULL)
    {
        fprintf(stderr, "Failed allocation with malloc\n");
        exit(LIST_EXIT_FAILED_ALLOCATION);
    }
    else
        return retval;
}

// ****************************
// Private
// ****************************

ListNode* ListNode_create(int value)
{
    ListNode* node = (ListNode*) allocate(sizeof(ListNode));

    node->value = value;
    node->next = LIST_NULLPTR;

    return node;
}

void ListNode_destroy(ListNode* node)
{
    free(node);
}

bool ListNode_is_last(ListNode* node)
{
    return (node->next == LIST_NULLPTR);
}

ListNode* ListNode_at_index(List* list, int index)
{

    if(index < -1)
    {
        fprintf(stderr, "Negative index access attempt\n");
        exit(LIST_EXIT_NEGATIVE_INDEX);
    }

    ListNode<T>* node = list;

    for(int i = -1; i < index ; i++)
    {
        if(ListNode_is_last(node))
        {
            fprintf(stderr, "Out of range access attempt\n");
            exit(LIST_EXIT_OUT_OF_BOUNDS);
        }

        node = node->next;
    }

    return node;
}

int List_endIndex(List* list)
{
    return List_size(list) - 1;
}

void ListNode_erase(ListNode** preErasedNodeNextReference)
{
    ListNode* erasedNode = *preErasedNodeNextReference;
    *preErasedNodeNextReference = erasedNode->next;
    ListNode_destroy(erasedNode);
}

// ****************************
// Create and destroy
// ****************************

List* List_init()
{
    List* list = (List*) allocate(sizeof(List));

    list->next = LIST_NULLPTR;

    return list;
}

void List_destroy(List* list)
{
    ListNode* next;

    for(ListNode* current = list; !ListNode_is_last(current); current = next)
    {
        next = current->next;
        ListNode_destroy(current);
    }
}

// ****************************
// Basic operations
// ****************************

int List_at(List* list, int index)
{
    if(index == -1)
    {
        fprintf(stderr, "Negative index access attempt\n");
        exit(LIST_EXIT_NEGATIVE_INDEX);
    }

    return ListNode_at_index(list, index)->value;
}

bool List_empty(List* list)
{
    return (ListNode_is_last(list));
}

void List_insert(List* list, int index, int value)
{
    ListNode* previous = ListNode_at_index(list, --index);
    ListNode* newNode = ListNode_create(value);
    newNode->next = previous->next;
    previous->next = newNode;
}

void List_erase(List* list, int index)
{
    ListNode_erase(&ListNode_at_index(list, index - 1)->next);
}

int List_size(List* list)
{
    int size = 0;

    for(ListNode* node = list; !ListNode_is_last(node); node = node->next)
        size++;

    return size;
}

// ****************************
// Derived operations
// ****************************

void List_push_front(List* list, int value)
{
    List_insert(list, 0, value);
}

int List_pop_front(List* list)
{
    int retval = List_at(list, 0);
    List_erase(list, 0);
    return retval;
}

void List_push_back(List* list, int value)
{
    List_insert(list, List_endIndex(list) + 1, value);
}

int List_pop_back(List* list)
{
    int endIndex = List_endIndex(list);

    int retval = List_at(list, endIndex);
    List_erase(list, endIndex);
    return retval;
}

int List_back(List* list)
{
    return List_at(list, List_endIndex(list));
}

int List_front(List* list)
{
    return List_at(list, 0);
}

int List_value_n_from_end(List* list, int reverseIndex)
{
    int index = List_endIndex(list) - reverseIndex;
    return List_at(list, index);
}

void List_remove_value(List* list, int value)
{
    for(ListNode** nodeNextReference = &list->next;
        *nodeNextReference != LIST_NULLPTR;
        nodeNextReference = &((*nodeNextReference)->next))

        if ((*nodeNextReference)->value == value)
            return ListNode_erase(nodeNextReference);
}

void List_reverse(List* list)
{
    // Quit if reverse is not required => list empty or contains only 1 element
    if (list->next == LIST_NULLPTR || list->next->next == LIST_NULLPTR)
        return;

    ListNode* current = list->next;
    list->next = LIST_NULLPTR;

    for(ListNode* next; next != LIST_NULLPTR; current = next)
    {
        next = current->next;
        current->next = list->next;
        list->next = current;
    }
}
