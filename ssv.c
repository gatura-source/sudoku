#include "main.h"

// Shared variables
int Sol[GRID_SIZE][GRID_SIZE];
int shared_row_status[GRID_SIZE];
int shared_col_status[GRID_SIZE];
int shared_sub_status[GRID_SIZE];

// Mutex and barrier for synchronization
pthread_mutex_t mutex;
pthread_barrier_t barrier;

void *validate_rows_and_subgrids(void *arg) {
    int thread_id = *((int *)arg);
    printf("Thread id %d\n", thread_id);
    free(arg);  // Free the argument memory

    Dict *row_dict[3] = { NULL };
    Dict *sub_dict[3] = { NULL };

    // Initialize dictionaries
    for (int i = 0; i < 3; i++) {
        row_dict[i] = (Dict *)malloc(sizeof(Dict));
        if (row_dict[i] == NULL) {
            // Handle memory allocation failure
            return NULL;
        }
        sub_dict[i] = (Dict *)malloc(sizeof(Dict));
        if (sub_dict[i] == NULL) {
            // Handle memory allocation failure
            for (int j = 0; j < i; j++) {
                free(row_dict[j]);
                free(sub_dict[j]);
            }
            return NULL;
        }
        for (int j = 0; j < TABLE_SIZE; j++) {
            row_dict[i]->table[j] = NULL;
            sub_dict[i]->table[j] = NULL;
        }
    }

    // Process rows and sub-grids for the assigned thread
    for (int i = 0; i < 3; i++) {
        int row_index = thread_id * 3 + i;
        int sub_index = thread_id * 3 + i;

        // Process row
        for (int j = 0; j < GRID_SIZE; j++) {
            char *str = NULL;
            int len = snprintf(NULL, 0, "%d", Sol[row_index][j]);
            str = (char *)malloc(len + 1);
            if (str == NULL) {
                printf("Mem Allocation failed\n");
                exit(EXIT_FAILURE);
            }
            snprintf(str, len + 1, "%d", Sol[row_index][j]);
            inc_val(row_dict[i], str);
            free(str);
        }
        pthread_mutex_lock(&mutex);
        shared_row_status[row_index] = sum(row_dict[i]);
        pthread_mutex_unlock(&mutex);

        // Process sub-grid
        int sub_row_start = (sub_index / 3) * 3;
        int sub_col_start = (sub_index % 3) * 3;
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                char *str = NULL;
                int len = snprintf(NULL, 0, "%d", Sol[sub_row_start + r][sub_col_start + c]);
                str = (char *)malloc(len + 1);
                if (str == NULL) {
                    perror("Mem Allocation failed");
                    exit(-1);
                }
                snprintf(str, len + 1, "%d", Sol[sub_row_start + r][sub_col_start + c]);
                inc_val(sub_dict[i], str);
                free(str);
            }
        }
        pthread_mutex_lock(&mutex);
        shared_sub_status[sub_index] = sum(sub_dict[i]);
        pthread_mutex_unlock(&mutex);
    }
    //Print
    // for (int j = 0; j < 3; j++)
    // {
    //     printf("Rows: \n");
    //     print(row_dict[j]);
    //     printf("Sub Grids: \n");
    //     print(sub_dict[j]);
    // }
    // Destroy dictionaries
    for (int i = 0; i < 3; i++) {
        destroy(row_dict[i]);
        destroy(sub_dict[i]);
    }

    pthread_barrier_wait(&barrier);
    return NULL;
}

void *validate_columns(void *arg) {
    Dict *col_dict[GRID_SIZE];

    // Initialize dictionaries
    for (int i = 0; i < GRID_SIZE; i++) {
        col_dict[i] = (Dict *)malloc(sizeof(Dict));
        for (int j = 0; j < TABLE_SIZE; j++) {
            col_dict[i]->table[j] = NULL;
        }
    }

    // Process columns
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            char str[4];
            snprintf(str, sizeof(str), "%d", Sol[j][i]);
            inc_val(col_dict[i], str);
        }
        shared_col_status[i] = sum(col_dict[i]);
    }

    // Destroy dictionaries
    for (int i = 0; i < GRID_SIZE; i++) {
        destroy(col_dict[i]);
    }

    pthread_barrier_wait(&barrier);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <solution_file> <delay>\n", argv[0]);
        return 1;
    }

    // Load Sudoku solution from file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error: Unable to open file %s\n", argv[1]);
        return 1;
    }
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            fscanf(fp, "%d", &Sol[i][j]);
        }
    }
    fclose(fp);

    // Initialize shared variables
    for (int i = 0; i < GRID_SIZE; i++) {
        shared_row_status[i] = 0;
        shared_col_status[i] = 0;
        shared_sub_status[i] = 0;
    }

    // Initialize mutex and barrier
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);

    // Create and start child threads
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        int *thread_id = (int *)malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&threads[i], NULL, validate_rows_and_subgrids, (void *)thread_id);
    }

    // Validate columns in the main thread
    validate_columns(NULL);

    // Wait for all threads to finish
    //pthread_barrier_wait(&barrier);

    // Print the results
    print_results();

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    return 0;
}

void print_results() {
    printf("Checking Results\n");
    int valid_count = 0;

    // Check row status
    printf("Row status: ");
    for (int i = 0; i < GRID_SIZE; i++) {
        if (shared_row_status[i] == 1) {
            valid_count++;
        } else {
            printf("Row %d is invalid, ", i + 1);
        }
    }
    printf("\n");

    // Check column status
    printf("Column status: ");
    for (int i = 0; i < GRID_SIZE; i++) {
        if (shared_col_status[i] == 1) {
            valid_count++;
        } else {
            printf("Column %d is invalid, ", i + 1);
        }
    }
    printf("\n");

    // Check sub-grid status
    printf("Sub-grid status: ");
    for (int i = 0; i < GRID_SIZE; i++) {
        if (shared_sub_status[i] == 1) {
            valid_count++;
        } else {
            printf("Sub-grid %d is invalid, ", i + 1);
        }
    }
    printf("\n");

    // Print the overall result
    if (valid_count == 27) {
        printf("There are %d valid sub-grids, and thus the solution is valid.\n", valid_count);
    } else {
        printf("There are %d valid sub-grids, and thus the solution is invalid.\n", valid_count);
    }
}