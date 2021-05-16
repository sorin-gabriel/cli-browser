#include <stdlib.h>

#ifndef _DATA_STRUCTURES_
#define _DATA_STRUCTURES_

/* MACRO-uri pentru manipularea structurilor de date
*/

#define STACK_EMPTY(S)      (((SP)S)->top == NULL)
#define STACK_TOP(S)        (((SP)S)->top)
#define STACK_DIM(S)        (((SP)S)->element_dim)

#define QUEUE_EMPTY(Q)      (((QP)Q)->first == NULL && ((QP)Q)->last == NULL)
#define QUEUE_FIRST(Q)      (((QP)Q)->first)
#define QUEUE_LAST(Q)       (((QP)Q)->last)
#define QUEUE_DIM(Q)        (((QP)Q)->element_dim)

#define DLL_EMPTY(L)        (((DCP)L)->next == (DCP)L)
#define DLL_SHIELD(L)       ((DCP)L)
#define DLL_NEXT(L)         (((DCP)L)->next)
#define DLL_PREV(L)         (((DCP)L)->prev)
#define DLL_DIM(L)          (*(size_t *)(((DCP)L)->info))

typedef struct generic_simple_cell {
    struct generic_simple_cell *next;
    void *info;
} cell, *point_to_cell, *CP, *list;

typedef struct generic_stack {
    size_t element_dim;
    point_to_cell top;
} stack, *point_to_stack, *SP;

typedef struct generic_queue {
    size_t element_dim;
    point_to_cell first, last;
} queue, *point_to_queue, *QP;

typedef struct generic_double_cell {
    struct generic_double_cell *next;
    struct generic_double_cell *prev;
    void *info;
} double_cell, *point_to_double_cell, *DCP, *double_list;

/* genereaza o stiva noua cu elemente de dimensiune @dimension
*/
void *newStack(size_t dimension);

/* copiaza informatiile de la adresa @element in varful stivei @my_stack
*/
int Push(void *my_stack, void *element);

/* elimina elementul din varful stivei @my_stack si copiaza-l la adresa @element
*/
int Pop(void *my_stack, void *element, void (*free_info)(void *));

/* distruge stiva de la adresa @address_of_stack si toate informatiile asociate
*/
void killStack(void **address_of_stack, void (*free_info)(void *));

/* genereaza o coada noua cu elemente de dimensiune @dimension
*/
void *newQueue(size_t dimension);

/* copiaza informatiile de la adresa @element la finalul cozii @my_queue
*/
int Enqueue(void *my_queue, void *element);

/* elimina primul element din coada @my_queue si copiaza-l la adresa @element
*/
int Dequeue(void *my_queue, void *element, void (*free_info)(void *));

/* distruge coada de la adresa @address_of_queue si toate informatiile asociate
*/
void killQueue(void **address_of_queue, void (*free_info)(void *));

/* genereaza o lista circulara cu santinela cu elemente de dimensiune @dimension
*/
void *newCircularDoubleList(size_t dimension);

/* copiaza informatiile de la adresa @element in campul ->next al celulei
de la adresa @my_list
*/
int insNext(void *my_list, void *element);

/* copiaza informatiile de la adresa @element in campul ->prev al celulei
de la adresa @my_list
*/
int insPrev(void *my_list, void *element);

/* elimina din lista celula indicata de campul ->next al celulei
de la adresa @my_list si copiaza informatiile la adresa @element
*/
int rmvNext(void *my_list, void *element, void (*free_info)(void *));

/* elimina din lista celula indicata de campul ->prev al celulei
de la adresa @my_list si copiaza informatiile la adresa @element
*/
int rmvPrev(void *my_list, void *element, void (*free_info)(void *));

/* distruge lista de la adresa @address_of_list si toate informatiile asociate
*/
void killCircularDoubleList(void **address_of_list, void (*free_info)(void *));

#endif