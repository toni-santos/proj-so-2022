#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

typedef struct {
    int rows;
    int columns;
} Dimension;

void add_matrices(const char* mx1, const char* mx2);
Dimension check_dimensions(const char* mx1, const char* mx2);
void read_matrix(const char* mx, int*** m, const Dimension* d);
void get_matrix_dimension(const char* mx, Dimension* d);
void print_matrix(int*** m, const Dimension* d);
void free_matrix(int*** m, const Dimension* d);
bool equal_dimensions(const Dimension* d1, const Dimension* d2);