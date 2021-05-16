#include "utils.h"
#include "structs.h"

#ifndef _BROWSER_
#define _BROWSER_

/* MACRO-uri pentru elementele din browser
*/

#define BYTES_LEFT(R)       (((Resource *)R)->dimension - \
                            ((Resource *)R)->currently_downloaded)
#define RES_NUM(W)          (((webpage *)W)->num_res)
#define RES_LIST(W)         (((webpage *)W)->res)

#define CRT_PAGE(T)         (((tab *)T)->current_page)
#define BCK_STACK(T)        (((tab *)T)->back_stack)
#define FWD_STACK(T)        (((tab *)T)->forward_stack)

#define CRT_TAB(B)          (((browser *)B)->current_tab)
#define DLOADS(B)           (((browser *)B)->downloads)
#define TABS(B)             (((browser *)B)->open_tabs)
#define HIST(B)             (((browser *)B)->history)

typedef struct webpage {
    char *url;
    int num_res;
    Resource *res;
} webpage;

typedef struct tab {
    webpage *current_page;
    void *back_stack;
    void *forward_stack;
} tab;

typedef struct browser {
    unsigned long bandwidth;
    tab *current_tab;
    void *open_tabs;
    void *downloads;
    void *history;
} browser;

/* sterge o pagina si toata memoria alocata ei
*/
void dealloc_page(void *my_webpage);

/* sterge un tab si toata memoria alocata lui
*/
void dealloc_tab(void *my_tab);

/* sterge un browser si toata memoria alocata lui
*/
void dealloc_browser(browser *my_browser);

/* genereaza o pagina noua cu adresa @url si ii incarca resursele
*/
webpage *create_webpage(char *url);

/* genereaza un tab nou, fara pagini deschise
*/
tab *create_tab();

/* genereaza un browser nou care contine un tab gol
*/
browser *create_browser();

/* schimba latimea de banda disponibila din DEFAULT_BANDWIDTH in @band_value
*/
void set_bandwidth(browser *my_browser, unsigned long band_value);

/* deschide un tab nou, gol, si comuta la el
*/
void open_tab(browser *my_browser);

/* inchide ultimul tab deschis
*/
void delete_tab(browser *my_browser);

/* schimba tabul curent cu tabul @tab_index
*/
void change_tab(browser *my_browser, long tab_index);

/* printeaza taburile in ordinea deschiderii lor
*/
void print_tabs(browser *my_browser, FILE *out_file_pointer);

/* schimba pagina curenta cu pagina de la adresa @url
*/
void go_to_url(browser *my_browser, char *url);

/* acceseaza pagina anterioara (daca exista)
*/
void back_page(browser *my_browser, FILE *out_file_pointer);

/* acceseaza pagina posterioara (daca exista)
*/
void forward_page(browser *my_browser, FILE *out_file_pointer);

/* printeaza istoricul, in ordinea accesarii paginilor
*/
void print_history(browser *my_browser, FILE *out_file_pointer);

/* sterge primele @entries din istoric
*/
void delete_history(browser *my_browser, unsigned long entries);

/* printeaza resursele existente in pagina curenta
*/
void print_resources(browser *my_browser, FILE *out_file_pointer);

/* printeaza lista de descarcari
*/
void print_downloads(browser *my_browser, FILE *out_file_pointer);

/* adauga resursa indicata de @index din pagina curenta in lista de descarcari
*/
void download_resource(browser *my_browser, unsigned long index);

/* actualizeaza starea resursei @my_resource din coada de descarcari
*/
long band_time(Resource *my_resource, unsigned long bandwidth);

/* separa coada de descarcari @down in doua stive, @finished si @working
*/
void split_download_queue(void *down, void *finished, void *working);

/* reasambleaza coada de descarcari @down din stivele @finished si @working
*/
void merge_download_queue(void *down, void *finished, void *working);

/* asteapta @time_to_wait secunde si actualizeaza descarcarile
*/
void wait(browser *my_browser, unsigned long time_to_wait);

#endif