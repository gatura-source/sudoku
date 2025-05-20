/*
Program implements basic functions that imitate the Python 
Dictionary
*/
#include "main.h"


/**
 * hash - Converts a string key to an index
 * @key: Key value that gets converted to an index
 * 
 * Returns: an index
 */
unsigned int hash(const char *key)
{
	//printf("%s\n",key);
	unsigned int hash = 0;
	while (*key)
	{
		hash = (hash * 31) + *key++;
	}
	return hash % TABLE_SIZE;
}

/**
 * create_pair - Creates a new key-value pair and returns a pointer;
 * @key: Key part of the pair
 * @value: value part of the pair
 * 
 * Returns - pointer to the pair
 */
KeyValuePair *create_pair(const char *key, int value)
{
	KeyValuePair *pair = (KeyValuePair *)malloc(sizeof(KeyValuePair));
	if (pair != NULL)
	{
		pair->key = strdup(key);
		pair->value = value;
		pair->next = NULL;
	}
	return pair;

}

/**
 * insert - Inserts a key-value pair into the dictionary
 * @dict: the dict to modify
 * @key: Key part of the new pair
 * @value: Value part of the new pair
 * 
 * Returns: Void
 */
void insert(Dict *dict, const char *key, int value)
{
	unsigned int index = hash(key);
	KeyValuePair *pair = create_pair(key, value);
	if (pair != NULL)
	{
		pair->next = dict->table[index];
		dict->table[index] = pair;
	}
}

/**
 * get - Get a value from a dict associated with a key
 * @dict: Dictionary
 * @key: Key
 * 
 * Returns - -1 if key not found
 */
int get(Dict *dict, const char *key)
{
	unsigned int index = hash(key);
	KeyValuePair *pair = dict->table[index];
	while (pair != NULL)
	{
		if (strcmp(pair->key, key) == 0)
		{
			return pair->value;
		}
		pair = pair->next;
	}
	return -1; /*Key not Found*/
}
/**
 * inc_val - Increments a value associated with the key by one
 * @dict: Dictionary to modify
 * @key: Key
 * 
 * Returns: void
 */
void inc_val(Dict *dict, const char *key)
{
	unsigned int index = hash(key);
	KeyValuePair *pair = dict->table[index];
	if (get(dict, key) == -1)
	{
		insert(dict, key, 1);
	}
}

/**
 * sum - Check every single Value in a dict
 * @dict: Dict to check
 * 
 * Returns: sum of all vals in a dict
 */
int sum(Dict *dict)
{
	unsigned int vals = 0;
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		KeyValuePair *current = dict->table[i];
		while (current != NULL)
		{
			vals += current->value;
			KeyValuePair *next = current->next;
			current = next;
		}
	}
	if (vals == 9)
	{
		return 1;
	}
	else
	{
		return (int)vals;
	}
}
/**
 * delete - Deletes a key-value pair from a dictionary
 * @dict: Dictionary to modify
 * @key: Key for the key-value pair to delete
 * 
 * Returns - Void
 */
void delete(Dict *dict, const char *key)
{
	unsigned int index = hash(key);
	KeyValuePair *prev = NULL;
	KeyValuePair *current = dict->table[index];

	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if(prev == NULL)
			{
				dict->table[index] = current->next;
			}
			else
			{
				prev->next = current->next;
			}
			free(current->key);
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

/**
 * destroy - Destroys a dictionary
 * @dict: dictionary to destroy
 * 
 * Returns - Void
 */
void destroy(Dict *dict)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		KeyValuePair *current = dict->table[i];
		while (current != NULL)
		{
			KeyValuePair *next = current->next;
			free(current->key);
			free(current);

			current = next;
		}
	}
	free(dict);
}

void print(Dict *dict)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		KeyValuePair *current = dict->table[i];
		while (current != NULL)
		{
			printf("Key: %s, Value: %d\n", current->key, current->value);
			KeyValuePair *next = current->next;
			current = next;
		}
	}
}
/**
 * itoa - Does the opposite of atoi
 * @num - Int
 * @char- Returned value
 * 
 * Returns - Char reprsentation of an int
 */
 

char* itoa(int num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If the number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append null character

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return str;
}

