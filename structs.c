#include <stdlib.h>
#include <string.h>

#include "structs.h"

void *newStack(size_t dim) {
    SP my_new_S = (SP)malloc(sizeof(stack));
    if (!my_new_S) return NULL;
    STACK_DIM(my_new_S) = dim;
    STACK_TOP(my_new_S) = NULL;
    return (void *)my_new_S;
}

int Push(void *my_S, void *el) {
    CP my_new_C = (CP)malloc(sizeof(cell));
    if (!my_new_C) return 0;
    my_new_C->next = STACK_TOP(my_S);
    STACK_TOP(my_S) = my_new_C;
    STACK_TOP(my_S)->info = malloc(STACK_DIM(my_S));
    memcpy(STACK_TOP(my_S)->info, el, STACK_DIM(my_S));
    return 1;
}

int Pop(void *my_S, void *el, void (*free_info)(void *)) {
    if (STACK_EMPTY(my_S)) return 0;
    if (el != NULL) memcpy(el, STACK_TOP(my_S)->info, STACK_DIM(my_S));
    CP current_top = STACK_TOP(my_S);
    STACK_TOP(my_S) = current_top->next;
    free_info(current_top->info);
    free(current_top);
    return 1;
}

void killStack(void **address_of_S, void (*free_info)(void *)) {
    while (Pop(*address_of_S, NULL, free_info));
    free(*address_of_S);
    *address_of_S = NULL;
}

void *newQueue(size_t dim) {
    QP my_new_Q = (QP)malloc(sizeof(queue));
    if (!my_new_Q) return NULL;
    QUEUE_DIM(my_new_Q) = dim;
    QUEUE_FIRST(my_new_Q) = NULL;
    QUEUE_LAST(my_new_Q) = NULL;
    return (void *)my_new_Q;
}

int Enqueue(void *my_Q, void *el) {
    CP my_new_C = (CP)malloc(sizeof(cell));
    if (!my_new_C) return 0;
    my_new_C->next = NULL;
    if (QUEUE_EMPTY(my_Q)) {
        QUEUE_LAST(my_Q) = my_new_C;
        QUEUE_FIRST(my_Q) = QUEUE_LAST(my_Q);
    } else {
        QUEUE_LAST(my_Q)->next = my_new_C;
        QUEUE_LAST(my_Q) = my_new_C;
    }
    QUEUE_LAST(my_Q)->info = malloc(QUEUE_DIM(my_Q));
    memcpy(QUEUE_LAST(my_Q)->info, el, QUEUE_DIM(my_Q));
    return 1;
}

int Dequeue(void *my_Q, void *el, void (*free_info)(void *)) {
    if (QUEUE_EMPTY(my_Q)) return 0;
    if (el != NULL) memcpy(el, QUEUE_FIRST(my_Q)->info, QUEUE_DIM(my_Q));
    CP current_first = QUEUE_FIRST(my_Q);
    QUEUE_FIRST(my_Q) = current_first->next;
    free_info(current_first->info);
    free(current_first);
    if (QUEUE_FIRST(my_Q) == NULL) QUEUE_LAST(my_Q) = NULL;
    return 1;
}

void killQueue(void **address_of_Q, void (*free_info)(void *)) {
    while (Dequeue(*address_of_Q, NULL, free_info));
    free(*address_of_Q);
    *address_of_Q = NULL;
}

void *newCircularDoubleList(size_t dim) {
    DCP my_new_L = (DCP)malloc(sizeof(double_cell));
    if (!my_new_L) return NULL;
    DLL_NEXT(my_new_L) = my_new_L;
    DLL_PREV(my_new_L) = my_new_L;
    my_new_L->info = malloc(sizeof(size_t));
    DLL_DIM(my_new_L) = dim;
    return (void *)my_new_L;
}

int insNext(void *my_L, void *el) {
    DCP my_new_C = (DCP)malloc(sizeof(double_cell));
    if (!my_new_C) return 0;
    if (DLL_EMPTY(my_L)) {
        DLL_NEXT(my_L) = my_new_C;
        DLL_PREV(my_L) = my_new_C;
        my_new_C->next = DLL_SHIELD(my_L);
        my_new_C->prev = DLL_SHIELD(my_L);
    } else {
        DLL_NEXT(my_L)->prev = my_new_C;
        my_new_C->next = DLL_NEXT(my_L);
        DLL_NEXT(my_L) = my_new_C;
        my_new_C->prev = DLL_SHIELD(my_L);
    }
    DLL_NEXT(my_L)->info = malloc(DLL_DIM(my_L));
    memcpy(DLL_NEXT(my_L)->info, el, DLL_DIM(my_L));
    return 1;
}

int insPrev(void *my_L, void *el) {
    DCP my_new_C = (DCP)malloc(sizeof(double_cell));
    if (!my_new_C) return 0;
    if (DLL_EMPTY(my_L)) {
        DLL_NEXT(my_L) = my_new_C;
        DLL_PREV(my_L) = my_new_C;
        my_new_C->next = DLL_SHIELD(my_L);
        my_new_C->prev = DLL_SHIELD(my_L);
    } else {
        DLL_PREV(my_L)->next = my_new_C;
        my_new_C->prev = DLL_PREV(my_L);
        DLL_PREV(my_L) = my_new_C;
        my_new_C->next = DLL_SHIELD(my_L);
    }
    DLL_PREV(my_L)->info = malloc(DLL_DIM(my_L));
    memcpy(DLL_PREV(my_L)->info, el, DLL_DIM(my_L));
    return 1;
}

int rmvNext(void *my_L, void *el, void (*free_info)(void *)) {
    if (DLL_EMPTY(my_L)) return 0;
    if (el != NULL) memcpy(el, DLL_NEXT(my_L)->info, DLL_DIM(my_L));
    DCP current_next = DLL_NEXT(my_L);
    DLL_NEXT(my_L) = current_next->next;
    DLL_NEXT(my_L)->prev = DLL_SHIELD(my_L);
    free_info(current_next->info);
    free(current_next);
    if (DLL_NEXT(my_L) == DLL_SHIELD(my_L)) {
        DLL_PREV(my_L) = DLL_SHIELD(my_L);
    }
    return 1;
}

int rmvPrev(void *my_L, void *el, void (*free_info)(void *)) {
    if (DLL_EMPTY(my_L)) return 0;
    if (el != NULL) memcpy(el, DLL_PREV(my_L)->info, DLL_DIM(my_L));
    DCP current_prev = DLL_PREV(my_L);
    DLL_PREV(my_L) = current_prev->prev;
    DLL_PREV(my_L)->next = DLL_SHIELD(my_L);
    free_info(current_prev->info);
    free(current_prev);
    if (DLL_PREV(my_L) == DLL_SHIELD(my_L)) {
        DLL_NEXT(my_L) = DLL_SHIELD(my_L);
    }
    return 1;
}

void killCircularDoubleList(void **address_of_L, void (*free_info)(void *)) {
    while (rmvNext(*address_of_L, NULL, free_info));
    free(&(DLL_DIM(*address_of_L)));
    free(*address_of_L);
    *address_of_L = NULL;
}