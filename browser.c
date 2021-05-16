#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "browser.h"
#include "structs.h"

/* valori implicite pentru latimea de banda si dimensiunea URL
*/

#define DEFAULT_BANDWIDTH 1024
#define URL_MAX_LEN 256

void dealloc_page(void *my_W) {
    free(((webpage *)my_W)->url);
    free(((webpage *)my_W)->res);
    free((webpage *)my_W);
    my_W = NULL;
}

void dealloc_tab(void *my_T) {
    if (CRT_PAGE(my_T)) dealloc_page(CRT_PAGE(my_T));
    if (BCK_STACK(my_T)) killStack(&BCK_STACK(my_T), dealloc_page);
    if (FWD_STACK(my_T)) killStack(&FWD_STACK(my_T), dealloc_page);
    free((tab *)my_T);
    my_T = NULL;
}

void dealloc_browser(browser *my_B) {
    if (TABS(my_B)) killCircularDoubleList(&(TABS(my_B)), dealloc_tab);
    if (DLOADS(my_B)) killQueue(&(DLOADS(my_B)), free);
    if (HIST(my_B)) killQueue(&(HIST(my_B)), free);
    free(my_B);
    my_B = NULL;
}

webpage *create_webpage(char *url) {
    webpage *my_new_W = (webpage *)malloc(sizeof(webpage));
    if (!my_new_W) return NULL;
    my_new_W->url = (char *)calloc(URL_MAX_LEN, sizeof(char));
    strcpy(my_new_W->url, url);
    my_new_W->res = get_page_resources(my_new_W->url, &(my_new_W->num_res));
    return my_new_W;
}

tab *create_tab() {
    tab *my_new_T = (tab *)malloc(sizeof(tab));
    if (!my_new_T) return NULL;
    CRT_PAGE(my_new_T) = NULL;
    BCK_STACK(my_new_T) = NULL;
    FWD_STACK(my_new_T) = NULL;
    return my_new_T;
}

browser *create_browser() {
    browser *my_new_B = (browser *)malloc(sizeof(browser));
    if (!my_new_B) return NULL;
    my_new_B->bandwidth = DEFAULT_BANDWIDTH;
    TABS(my_new_B) = newCircularDoubleList(sizeof(tab));
    DLOADS(my_new_B) = newQueue(sizeof(Resource));
    HIST(my_new_B) = newQueue(URL_MAX_LEN * sizeof(char));
    open_tab(my_new_B);
    return my_new_B;
}

void set_bandwidth(browser *my_B, unsigned long value) {
    my_B->bandwidth = value;
}

void open_tab(browser *my_B) {
    void *my_new_T = (void *)create_tab();
    insNext(TABS(my_B), my_new_T);
    free(my_new_T);
    change_tab(my_B, -1);
}

void delete_tab(browser *my_B) {
    if (CRT_TAB(my_B) == DLL_NEXT(TABS(my_B))->info) change_tab(my_B, -2);
    rmvNext(TABS(my_B), NULL, dealloc_tab);
}

/* pentru @index >= 0 se parcurge lista de la primul tab deschis catre ultimul,
iar pentru @index < 0 se parcurge lista in sens invers, de la cel mai recent
tab deschis catre primul; de exemplu, @index = -1 este ultimul tab deschis,
@index = -2 este penultimul, @index = 0 este primul, @index = 1 este al doilea
*/

void change_tab(browser *my_B, long index) {
    DCP cell_pointer = DLL_SHIELD(TABS(my_B));
    if (index < 0) {
        long count = 0;
        while (count-- != index) cell_pointer = cell_pointer->next;
    } else {
        long count = -1;
        while (count++ != index) cell_pointer = cell_pointer->prev;
    }
    CRT_TAB(my_B) = (tab*)(cell_pointer->info);
}

void print_tabs(browser *my_B, FILE *out_F) {
    long index = 0;
    DCP print_T = DLL_PREV(TABS(my_B));
    while (print_T != DLL_SHIELD(TABS(my_B))) {
        if (CRT_PAGE(print_T->info)) {
            fprintf(out_F, "(%ld: %s)\n", index, \
                CRT_PAGE(print_T->info)->url);
        } else {
            fprintf(out_F, "(%ld: empty)\n", index);
        }
        index++;
        print_T = print_T->prev;
    }
}

void go_to_url(browser *my_B, char *url) {
    if (CRT_PAGE(CRT_TAB(my_B)) != NULL) {
        if (!BCK_STACK(CRT_TAB(my_B))) {
            BCK_STACK(CRT_TAB(my_B)) = newStack(sizeof(webpage));
        }
        Push(BCK_STACK(CRT_TAB(my_B)), CRT_PAGE(CRT_TAB(my_B)));
        free(CRT_PAGE(CRT_TAB(my_B)));
    }
    if (FWD_STACK(CRT_TAB(my_B))) {
        killStack(&FWD_STACK(CRT_TAB(my_B)), dealloc_page);
        FWD_STACK(CRT_TAB(my_B)) = NULL;
    }
    CRT_PAGE(CRT_TAB(my_B)) = create_webpage(url);
    Enqueue(HIST(my_B), url);
    wait(my_B, 1);
}

void back_page(browser *my_B, FILE *out_F) {
    if (!BCK_STACK(CRT_TAB(my_B)) || STACK_EMPTY(BCK_STACK(CRT_TAB(my_B)))) {
            fprintf(out_F, "can't go back, no pages in stack\n");
    } else {
        if (!FWD_STACK(CRT_TAB(my_B))) {
            FWD_STACK(CRT_TAB(my_B)) = newStack(sizeof(webpage));
        }
        Push(FWD_STACK(CRT_TAB(my_B)), CRT_PAGE(CRT_TAB(my_B)));
        Pop(BCK_STACK(CRT_TAB(my_B)), CRT_PAGE(CRT_TAB(my_B)), free);
    }
}

void forward_page(browser *my_B, FILE *out_F) {
    if (!FWD_STACK(CRT_TAB(my_B)) || STACK_EMPTY(FWD_STACK(CRT_TAB(my_B)))) {
            fprintf(out_F, "can't go forward, no pages in stack\n");
    } else {
        if (!BCK_STACK(CRT_TAB(my_B))) {
            BCK_STACK(CRT_TAB(my_B)) = newStack(sizeof(webpage));
        }
        Push(BCK_STACK(CRT_TAB(my_B)), CRT_PAGE(CRT_TAB(my_B)));
        Pop(FWD_STACK(CRT_TAB(my_B)), CRT_PAGE(CRT_TAB(my_B)), free);
    }
}

void print_history(browser *my_B, FILE *out_F) {
    void *print_Q = newQueue(QUEUE_DIM(HIST(my_B)));
    char *url_buffer = (char *)malloc(QUEUE_DIM(HIST(my_B)));
    while (Dequeue(HIST(my_B), url_buffer, free)) Enqueue(print_Q, url_buffer);
    while (Dequeue(print_Q, url_buffer, free)) {
        fprintf(out_F, "%s\n", url_buffer);
        Enqueue(HIST(my_B), url_buffer);
    }
    killQueue(&print_Q, free);
    free(url_buffer);
}

void delete_history(browser *my_B, unsigned long entries) {
    if (entries == 0) while (Dequeue(HIST(my_B), NULL, free));
    else {
        unsigned long count = 0;
        while (count++ != entries && Dequeue(HIST(my_B), NULL, free));
    }
}

void print_resources(browser *my_B, FILE *out_F) {
    int index;
    if (CRT_PAGE(CRT_TAB(my_B)) && RES_LIST(CRT_PAGE(CRT_TAB(my_B)))) {
        for (index = 0; index < RES_NUM(CRT_PAGE(CRT_TAB(my_B))); index++) {
            fprintf(out_F, "[%d - \"%s\" : %lu]\n", index, \
                RES_LIST(CRT_PAGE(CRT_TAB(my_B)))[index].name, \
                RES_LIST(CRT_PAGE(CRT_TAB(my_B)))[index].dimension);
        }
    }
}

void print_downloads(browser *my_B, FILE *out_F) {
    void *print_Q = newQueue(QUEUE_DIM(DLOADS(my_B)));
    Resource *buffer = (Resource *)malloc(QUEUE_DIM(DLOADS(my_B)));
    while (Dequeue(DLOADS(my_B), buffer, free)) Enqueue(print_Q, buffer);
    while (Dequeue(print_Q, buffer, free)) {
        if (!BYTES_LEFT(buffer)) {
            fprintf(out_F, "[\"%s\" : completed]\n", buffer->name);
        } else {
            fprintf(out_F, "[\"%s\" : %ld/%ld]\n", buffer->name, \
                BYTES_LEFT(buffer), buffer->dimension);
        }
        Enqueue(DLOADS(my_B), buffer);
    }
    killQueue(&print_Q, free);
    free(buffer);
}

/* pentru a mentine lista de descarcari ordonata, o resursa noua se insereaza
dupa ultima resursa care are mai putini bytes de descarcat decat resursa care
se doreste a fi inserata
*/

void download_resource(browser *my_B, unsigned long index) {
    if (CRT_PAGE(CRT_TAB(my_B)) && RES_LIST(CRT_PAGE(CRT_TAB(my_B)))) {
        Resource crt_res = RES_LIST(CRT_PAGE(CRT_TAB(my_B)))[index];
        Resource *buffer = (Resource *)malloc(QUEUE_DIM(DLOADS(my_B)));
        void *priority = newQueue(QUEUE_DIM(DLOADS(my_B)));
        while (Dequeue(DLOADS(my_B), buffer, free)) Enqueue(priority, buffer);
        while (!QUEUE_EMPTY(priority) && BYTES_LEFT(&crt_res) > \
            BYTES_LEFT(QUEUE_FIRST(priority)->info) && \
            BYTES_LEFT(QUEUE_FIRST(priority)->info)) {
            Dequeue(priority, buffer, free);
            Enqueue(DLOADS(my_B), buffer);
        }
        Enqueue(DLOADS(my_B), &crt_res);
        while (Dequeue(priority, buffer, free)) Enqueue(DLOADS(my_B), buffer); 
        killQueue(&priority, free);
        free(buffer);
    }
}

long band_time(Resource *my_R, unsigned long band) {
    if (BYTES_LEFT(my_R) >= band) {
        my_R->currently_downloaded += band;
        return 0;
    } else {
        unsigned long bytes_left = BYTES_LEFT(my_R);
        my_R->currently_downloaded = my_R->dimension;
        return band - bytes_left;
    }
}

/* separarea cozii de descarcari @down in doua stive ordonate special,
@finished si @working se realizeaza pentru a prelucra mai usor resursele a
caror descarcare nu s-a finalizat
*/

void split_download_queue(void *down, void *finished, void *working) {
    Resource *buffer = (Resource *)malloc(QUEUE_DIM(down));
    while (Dequeue(down, buffer, free)) Push(working, buffer);
    while (Pop(working, buffer, free)) Enqueue(down, buffer);
    while (!QUEUE_EMPTY(down) && !BYTES_LEFT(QUEUE_FIRST(down)->info)) {
        Dequeue(down, buffer, free);
        Push(finished, buffer);
    }
    while (Dequeue(down, buffer, free)) Push(working, buffer);
    while (Pop(finished, buffer, free)) Enqueue(down, buffer);
    while (Dequeue(down, buffer, free)) Push(finished, buffer);
    free(buffer);
}

/* reasamblarea cozii de descarcari se realizeaza tinand cont de schimbarile
produse (finalizarea anumitor descarcari)
*/

void merge_download_queue(void *down, void *finished, void *working) {
    Resource *buffer = (Resource *)malloc(QUEUE_DIM(down));
    while (Pop(finished, buffer, free)) Enqueue(down, buffer);
    while (Dequeue(down, buffer, free)) Push(finished, buffer);
    while (Pop(working, buffer, free)) Enqueue(down, buffer);
    while (Pop(finished, buffer, free)) Enqueue(down, buffer);
    free(buffer);
}

void wait(browser *my_B, unsigned long time_to_wait) {    
    void *finished = newStack(QUEUE_DIM(DLOADS(my_B)));
    void *working = newStack(QUEUE_DIM(DLOADS(my_B)));
    Resource *buffer = (Resource *)malloc(QUEUE_DIM(DLOADS(my_B)));
    split_download_queue(DLOADS(my_B), finished, working);
    unsigned long clock = 0;
    unsigned long remainder = 0;
    while (clock++ != time_to_wait && !STACK_EMPTY(working)) {
        Pop(working, buffer, free);
        remainder = band_time(buffer, remainder + my_B->bandwidth);
        if (!BYTES_LEFT(buffer)) Push(finished, buffer);
        else Push(working, buffer);
    }
    merge_download_queue(DLOADS(my_B), finished, working);
    killStack(&working, free);
    killStack(&finished, free);
    free(buffer);
}