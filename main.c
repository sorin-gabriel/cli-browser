#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "browser.h"

#define LINE_DIMENSION 300
#define DELIMS " \n"

/* scrie urmatoarea instructiune din fisierul @in_F in bufferul @task
*/
size_t get_next_task(char *task, FILE *in_F) {
    size_t task_size_max = LINE_DIMENSION;
    size_t task_size = getline(&task, &task_size_max, in_F);
    return task_size;
}

/* executa instructiunea indicata de @buffer
*/
void execute_task(browser *my_B, char *buffer, FILE *out_F) {
    char *task = strtok(buffer, " \n");
    if (!strcmp(task, "set_band")) {
        set_bandwidth(my_B, strtoul(strtok(NULL, DELIMS), NULL, 10));
    }
    if (!strcmp(task, "newtab")) open_tab(my_B);
    if (!strcmp(task, "deltab")) delete_tab(my_B);
    if (!strcmp(task, "change_tab")) {
        change_tab(my_B, strtol(strtok(NULL, DELIMS), NULL, 10));
    }
    if (!strcmp(task, "print_open_tabs")) print_tabs(my_B, out_F);
    if (!strcmp(task, "goto")) go_to_url(my_B, strtok(NULL, DELIMS));
    if (!strcmp(task, "back")) back_page(my_B, out_F);
    if (!strcmp(task, "forward")) forward_page(my_B, out_F);
    if (!strcmp(task, "history")) print_history(my_B, out_F);
    if (!strcmp(task, "del_history")) {
        delete_history(my_B, strtoul(strtok(NULL, DELIMS), NULL, 10));
    }
    if (!strcmp(task, "list_dl")) print_resources(my_B, out_F);
    if (!strcmp(task, "downloads")) print_downloads(my_B, out_F);
    if (!strcmp(task, "download")) {
        download_resource(my_B, strtoul(strtok(NULL, DELIMS), NULL, 10));
    }
    if (!strcmp(task, "wait")) {
        wait(my_B, strtoul(strtok(NULL, DELIMS), NULL, 10));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "\n(!) Malformed command parameters\n");
        return -1;
    }

    FILE *in_file, *out_file;
    in_file = fopen(argv[1], "r");
    out_file = fopen(argv[2], "w");    
    if (!in_file || !out_file) {
        fprintf(stderr, "\n(!) Unable to create I/O link\n");
        return -2;
    }

    browser *my_browser = create_browser();
    char *task_buffer = (char *)calloc(LINE_DIMENSION, sizeof(char));
    while (in_file && out_file) {
        size_t task_len = get_next_task(task_buffer, in_file);
        size_t end_of_instruction_set = -1;
        if (task_len == end_of_instruction_set) {
            fclose(in_file);
            in_file = NULL;
            fclose(out_file);
            out_file = NULL;
        } else {
            execute_task(my_browser, task_buffer, out_file);
        }
    }
    dealloc_browser(my_browser);
    free(task_buffer);
    return 0;
}