#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define GRID_SIZE 9
#define NUM_THREADS 4
#define TABLE_SIZE 100

//Hashmap Data structure
typedef struct KeyValuePair {
	char *key;
	int value;
	struct KeyValuePair *next;
}KeyValuePair;

typedef struct Dict {
	KeyValuePair *table[TABLE_SIZE];
} Dict;



// Function prototypes
void *thread1_func(void *arg);
void *thread2_func(void *arg);
void *thread3_func(void *arg);
void *thread4_func(void *arg);
void print_results();
unsigned int hash(const char *);
void destroy(Dict *);
KeyValuePair *create_pair(const char *, int);
int get(Dict *, const char *);
void insert(Dict *, const char *, int);
void delete(Dict *, const char *);
void inc_val(Dict *, const char *);
int sum(Dict *);
char* itoa(int , char *, int);
void print(Dict *);



#endif