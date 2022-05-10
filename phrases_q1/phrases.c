#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>

#include "phrases.h"

const char phrase_end[] = {'.', '!', '?'};

bool find(const char c) {
    for (int i = 0; i < 3; i++){
        if (c == phrase_end[i]) return true;
    }

    return false;
}

int main(int argc, char** argv) {
    int opt;
    int print_lines = 0;

    if (argc > 3) {
        printf("usage: phrases [-l] file");
        return EXIT_FAILURE;
    }

    opt = getopt(argc, argv, ":l:");

    switch (opt) {
    case 'l':
        print_lines = 1;
        print_phrases(argv, print_lines);
        break;
    case '?':
        printf("unknown option: %c\n", opt);
        break;
    default:
        count_phrases(argv);
        break;
    }


    return EXIT_SUCCESS;
}

int print_phrases(char** argv, int lines) {

    int counter = 1;
    FILE* file = fopen(argv[2], "r");
    char curr_char, last_char;
    int printed_counter = 0;

    if (file == NULL) {
        fprintf(stderr, "invalid file: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    while ((curr_char = fgetc(file)) != EOF) {

        if (!printed_counter && lines) {
            printf("[%d] ", counter);
            printed_counter = 1;
        }


        if (!find(curr_char) && find(last_char)) {
            counter++;
            if (lines) {
                printf("\n");
                printed_counter = 0;
            }
            last_char = curr_char;
            continue;
        }

        if (lines) {
            putchar(curr_char);
        }

        last_char = curr_char;
    }

    if (!lines) {
        printf("%d\n", counter);
    }

    fclose(file);
    return 0;
}

int count_phrases(char** argv) {
    int counter = 1;
    FILE* file = fopen(argv[1], "r");
    char curr_char, last_char;

    if (file == NULL) {
        fprintf(stderr, "Invalid file: %s\n", strerror(errno));
    }

    while ((curr_char = fgetc(file)) != EOF) {

        if (!find(curr_char) && find(last_char)) {
            counter++;
        }

        last_char = curr_char;
    }

    printf("%d\n", counter);

    return 0;
}