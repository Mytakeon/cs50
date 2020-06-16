// Implements a dictionary's functionality
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N_BUCKETS = 26;

// Hash table
node *table[N_BUCKETS];

// Number of words in the dictionary
int LOADED_WORDS = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *current_node = table[hash(word)];

    // Check first node
    if (strcasecmp(current_node->word, word) == 0)
    {
        return true;
    }

    // Traverse rest of the linked list
    while (current_node->next != NULL)
    {
        current_node = current_node->next;
        if (strcasecmp(current_node->word, word) == 0)
        {
            return true;
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    return (int)tolower(word[0]) - 97;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    unsigned int index;

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char *word = malloc(LENGTH);
    if (word == NULL)
    {
        return false;
    }

    while (fscanf(file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        LOADED_WORDS++;

        strcpy(new_node->word, word);

        index = hash(word);

        // Prepend the new node to the linked list.
        new_node->next = table[index];
        table[index] = new_node;
    }

    fclose(file);
    free(word);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return LOADED_WORDS;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Use two nodes to not lose track of the next node when freeing memory
    node *current_node;
    node *temp_node;

    for (int i = 0; i < N_BUCKETS; i++)
    {
        if (table[i] != NULL)
        {
            current_node = table[i];
            temp_node = current_node;

            while (current_node->next != NULL)
            {
                current_node = current_node->next;
                free(temp_node);
                temp_node = current_node;
            }

            free(current_node);
        }
    }

    return true;
}