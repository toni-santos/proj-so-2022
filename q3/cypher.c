#include "cypher.h"

int main(int argc, char* argv[]) {

    int child_to_parent[2];
    int parent_to_child[2];
    pid_t pid;
    int init_size = 5;

    Array cyphers;

    int n_buffers = 0;
    char* buffers[1000]; // please don't give me more than 1000 cyphers

    init_array(&cyphers, init_size);
    read_cyphers(&cyphers, buffers, &n_buffers);

    if (pipe(child_to_parent) < 0) {
        fprintf(stderr, "Error creating pipe, exiting...\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(parent_to_child) < 0) {
        fprintf(stderr, "Error creating pipe, exiting...\n");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) { // child process

        close(parent_to_child[WRITE_END]);
        close(child_to_parent[READ_END]);

        char received[30];
        char delim;

        while (read(parent_to_child[READ_END], received, 30)) {

            delim = '0';

            if (received[strlen(received) - 1] == '.' || received[strlen(received) - 1] == ',' || received[strlen(received) - 1] == '!') {
                delim = received[strlen(received) - 1];
                received[strlen(received) - 1] = '\0';
            }
            
            for (int i = 0; i < cyphers.used; i++) {

                if (strcmp(received, cyphers.array[i].word1) == 0) {
                    memset(received, 0, 30);
                    strcpy(received, cyphers.array[i].word2);
                    break;
                }
                else if (strcmp(received, cyphers.array[i].word2) == 0) {
                    memset(received, 0, 30);
                    strcpy(received, cyphers.array[i].word1);
                    break;
                }

            }

            if (delim != '0') received[strlen(received)] = delim;
            if (received[strlen(received) - 1] != '\n') strcat(received, " ");

            write(child_to_parent[WRITE_END], received, strlen(received));

            memset(received, 0, 30);
        }



        close(child_to_parent[WRITE_END]);
        close(parent_to_child[READ_END]);

        free_array(&cyphers);

        for (int i = 0; i < n_buffers; i++) {
            free(buffers[i]);
        }

        exit(0);

    }
    else { // parent process

        close(child_to_parent[1]);
        close(parent_to_child[0]);

        char line[MAX_LINE_SIZE + 1];
        char received[30];
        char* token;

        while (fgets(line, MAX_LINE_SIZE, stdin)) {

            char* buffer = strdup(line);
            token = strtok(buffer, " ");

            while (token != NULL) {

                write(parent_to_child[WRITE_END], token, strlen(token));
                token = strtok(NULL, " ");
                read(child_to_parent[READ_END], received, 30);

                printf("%s", received);
                memset(received, 0 , 30);

            }


        }

        close(parent_to_child[WRITE_END]);
        close(child_to_parent[READ_END]);

        wait(NULL);

    }


    free_array(&cyphers);

    for (int i = 0; i < n_buffers; i++) {
        free(buffers[i]);
    }

    return EXIT_SUCCESS;
}

void init_array(Array* a, size_t init_size) {

    a->array = malloc(sizeof(Pair) * init_size);
    a->size = init_size;
    a->used = 0;

}

void insert_array(Array* a, Pair element) {

    if (a->used == a->size) {
        a->size *= 2;
        a->array = realloc(a->array, sizeof(Pair) * a->size);
    }

    a->array[a->used++] = element;

}

void free_array(Array* a) {
    free(a->array);
    a->array = NULL;
    a->used = 0;
    a->size = 0;
}

void read_cyphers(Array* a, char** buffers, int* n_buffers) {

    FILE* file = fopen("cypher.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening cypher.txt file, exiting...\n");
        exit(EXIT_FAILURE);
    }

    char buffer[100]; //assuming we aren't given words longer than 100 characters

    while (fgets(buffer, 100, file)) {

        Pair new_pair;

        char* nbuffer = malloc(strlen(buffer));
        strcpy(nbuffer, buffer);
        buffers[(*n_buffers)++] = nbuffer;

        new_pair.word1 = strtok(nbuffer, " \n");
        new_pair.word2 = strtok(NULL, " \n");

        insert_array(a, new_pair);

    }

    fclose(file);
}