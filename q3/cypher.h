#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

#define MAX_LINE_SIZE 512

typedef struct word_pair {
    char* word1;
    char* word2;
} Pair;

typedef struct {
    Pair* array;
    size_t used;
    size_t size;
} Array;

void init_array(Array* a, size_t init_size);
void insert_array(Array* a, Pair element);
void free_array(Array* a);

void read_cyphers(Array* a, char** buffers, int* n_buffers);