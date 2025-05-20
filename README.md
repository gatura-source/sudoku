# sudoku
Learning IPC using a sudoku checker

# Sudoku Validator

This project contains a C program that validates a Sudoku solution using multithreading and a custom hash table implementation.

## Table of Contents

- [Description](#description)
- [Files](#files)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
  - [Hash Table](#hash-table)
  - [Multithreading and Synchronization](#multithreading-and-synchronization)
- [Dependencies](#dependencies)

## Description

The Sudoku Validator program takes a Sudoku solution as input and checks whether it is valid or not. It verifies that each row, column, and 3x3 sub-grid contains all digits from 1 to 9 without duplicates.

The program uses multithreading to parallelize the validation process, with each thread responsible for validating a subset of rows and sub-grids. A separate thread is used to validate the columns. Synchronization mechanisms, such as mutexes and barriers, are employed to ensure thread safety and correct data access.

Additionally, the program includes a custom implementation of a hash table (or dictionary) data structure, which is used to efficiently count the occurrences of values in rows, columns, and sub-grids.

## Files

- `hashmap.c`: Implements the hash table (dictionary) data structure and associated operations.
- `mssv.c`: Contains the main Sudoku Validator program, including the multithreaded validation logic and synchronization.
- `main.h`: Header file containing necessary definitions and function prototypes.

## Usage

To compile the program, use the following command:
- gcc -o mssv hashmap.c mssv.c -lpthread

Alternatively you can use the Makefile for compilation as shown below:
- **make**

Run the compiled program with the following command:
- ./validator <solution_file> <delay>

- `<solution_file>`: Path to the file containing the Sudoku solution (9x9 grid).
- `<delay>`: An optional delay (in milliseconds) to simulate processing time for demonstration purposes.

The program will output the validation status for each row, column, and sub-grid, as well as an overall result indicating whether the solution is valid or invalid.

## Implementation Details

### Hash Table

The hash table implementation in `hashmap.c` provides the following functions:

- `hash`: Computes a hash value for a given string key.
- `create_pair`: Creates a new key-value pair.
- `insert`: Inserts a key-value pair into the dictionary.
- `get`: Retrieves the value associated with a given key.
- `inc_val`: Increments the value associated with a given key.
- `sum`: Computes the sum of all values in the dictionary.
- `delete`: Deletes a key-value pair from the dictionary.
- `destroy`: Deallocates all memory used by the dictionary.
- `print`: Prints all key-value pairs in the dictionary.
- `itoa`: Converts an integer to a string representation.

### Multithreading and Synchronization

The `mssv.c` file contains the main Sudoku Validator program, which uses multithreading and synchronization mechanisms to validate the Sudoku solution:

- `validate_rows_and_subgrids`: This function is executed by multiple threads, each responsible for validating a subset of rows and sub-grids using dictionaries.
- `validate_columns`: This function is executed by the main thread and validates all columns using dictionaries.
- Synchronization is achieved using a mutex (`pthread_mutex_t`) to protect shared status variables and a barrier (`pthread_barrier_t`) to ensure all threads complete before printing the final results.

## Dependencies

The program requires the following dependencies:

- POSIX Threads (`pthread`) library for multithreading support.

On most Unix-like systems, you can link against the `pthread` library by adding the `-lpthread` flag during compilation.

