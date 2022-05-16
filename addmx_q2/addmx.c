#include "addmx.h"

int main(int argc, char** argv) {
    int opt;

    if (argc != 3) {
        printf("usage: addmx matrix_1 matrix_2");
        return EXIT_FAILURE;
    }

    opt = getopt(argc, argv, "::");

    switch (opt) {
    case '?':
        printf("unknown option: %c\n", opt);
        break;
    default:
        add_matrices(argv[1], argv[2]);
        break;
    }


    return EXIT_SUCCESS;
}

void add_matrices(const char* mx1, const char* mx2) {

    Dimension d = check_dimensions(mx1, mx2);

    int** m1;
    m1 = (int**)malloc(sizeof(int*) * d.rows);
    for (int i = 0; i < d.rows; i++) {
        m1[i] = (int*)malloc(sizeof(int) * d.columns);
    }

    int** m2;
    m2 = (int**)malloc(sizeof(int*) * d.rows);
    for (int i = 0; i < d.rows; i++) {
        m2[i] = (int*)malloc(sizeof(int) * d.columns);
    }

    read_matrix(mx1, &m1, &d);
    read_matrix(mx2, &m2, &d);

    int** result;

    result = (int**)mmap(NULL, sizeof(int*) * d.rows, PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, 0, 0);
    if (result == MAP_FAILED) {
        fprintf(stderr, "mmap() failed #1, exiting...\n");
        exit(-1);
    }

    for (int i = 0; i < d.rows; i++) {
        result[i] = (int*)mmap(NULL, sizeof(int) * d.columns, PROT_READ | PROT_WRITE, MAP_SHARED | 0x20, 0, 0);
        if (result[i] == MAP_FAILED) {
            fprintf(stderr, "mmap() failed #2, exiting...\n");
            exit(-1);
        }
    }

    for (int i = 0; i < d.columns; i++) {

        if (fork() == 0) {

            for (int j = 0; j < d.rows; j++) {
                result[j][i] = m2[j][i] + m1[j][i];
            }
            
            free_matrix(&m1, &d);
            free_matrix(&m2, &d);

            exit(0);

        }
    }

    while (wait(NULL) > 0) { continue; } // waiting for all child processes to end

    print_matrix(&result, &d);

    free_matrix(&m1, &d);
    free_matrix(&m2, &d);

    for (int i = 0; i < d.rows; i++) {
        munmap(result[i], sizeof(result[i]));
    }
    munmap(result, sizeof(result)); 
}


void read_matrix(const char* mx, int*** m, const Dimension* d) {

    FILE* file = fopen(mx, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file for reading matrix, exiting...\n");
        exit(EXIT_FAILURE);
    }

    int line_size = d->columns * d->columns;
    char buffer[line_size];
    char* token;

    fgets(buffer, line_size, file); // skip the dimension row

    for (int i = 0; i < d->rows; i++) {

        fgets(buffer, line_size, file);
        token = strtok(buffer, " ");

        for (int j = 0; j < d->columns; j++) {
            (*m)[i][j] = atoi(token);
            token = strtok(NULL, " ");
        }

    }

    fclose(file);
}

Dimension check_dimensions(const char* mx1, const char* mx2) {

    Dimension d1, d2;

    get_matrix_dimension(mx1, &d1);
    get_matrix_dimension(mx2, &d2);

    if (!equal_dimensions(&d1, &d2)) {
        fprintf(stderr, "Unable to add matrices of different dimensions!\n");
        exit(EXIT_FAILURE);
    }

    return d1;
}

void get_matrix_dimension(const char* mx, Dimension* d) {

    FILE* file = fopen(mx, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file for getting dimensions, exiting...\n");
        exit(EXIT_FAILURE);
    }

    d->rows = fgetc(file) - '0';
    fgetc(file); //skipping the 'x' characters
    d->columns = fgetc(file) - '0';

    fclose(file);
}

void print_matrix(int*** m, const Dimension* d) {

    printf("%dx%d\n", d->rows, d->columns);
    for (int i = 0; i < d->rows; i++) {

        for (int j = 0; j < d->columns; j++) {
            printf("%d ", (*m)[i][j]);
        }
        printf("\n");

    }
}

void free_matrix(int*** m, const Dimension* d) {
    
    for (int i = 0; i < d->rows; i++) {
        free((*m)[i]);
    }
    free(*m);

}

bool equal_dimensions(const Dimension* d1, const Dimension* d2) {
    return (d1->rows == d2->rows && d1->columns == d2->columns);
}