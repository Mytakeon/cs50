#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

void get_ciphertext(string text, string key);
bool isvalid(string key, int size);


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key");
        return 1;
    }

    string key = argv[1];
    int n = strlen(key);

    if (!isvalid(key, n))
    {
        return 1;
    }

    string text = get_string("plaintext: ");
    printf("ciphertext: ");
    get_ciphertext(text, key);
}

void get_ciphertext(string text, string key)
{
    char c;
    int size = strlen(text);

    for (int i = 0; i < size; i++)
    {
        c = text[i];
        if (isalpha(c))
        {
            int index = tolower(c) - 'a';  // index of character in alphabet
            char new_letter = key[index];

            // print instead the corresponding key. Be case sensitive.
            if (islower(c))
            {
                printf("%c", tolower(new_letter));

            }
            else
            {
                printf("%c", toupper(new_letter));
            }
        }
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");
}

bool isvalid(string key, int size)
{

    int const KEYSIZE = 26;

    if (size != KEYSIZE)
    {
        printf("Key is not valid; must contain 26 characters.");
        return false;
    }

    // Very silly brute force check. Would be better to use a hash map and to traverse the string only once.
    for (int i = 0; i < size; i++)
    {
        char c = key[i];
        if (!isalpha(c))
        {
            printf("Key is not valid; contains non-alphanumeric characters");
            return false;
        }

        for (int j = 0; j < size; j++)
        {
            if (j != i && c == key[j])
            {
                printf("Key is not valid; contains duplicate character (%c)", c);
                return false;
            }
        }
    }
    return true;

}