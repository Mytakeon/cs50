#include <cs50.h>
#include <stdio.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
void compute_grade(string text);


int main(void)
{
    string text = get_string("Text: ");
    compute_grade(text);

}

int count_letters(string text)
{
    int i = 0;
    int n_letters = 0;
    char c;

    do
    {
        c = text[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            n_letters++;
        }
        i++;
    }
    while (c != '\0');
    return n_letters;
}

int count_words(string text)
{
    int i = 0;
    int n_words = 1;
    char c;

    do
    {
        c = text[i];
        if (c == ' ')
        {
            n_words++;
        }
        i++;
    }
    while (c != '\0');
    return n_words;
}

int count_sentences(string text)
{
    int i = 0;
    int n_sentences = 0;
    char c;

    do
    {
        c = text[i];
        if (c == '.' || c == '!' || c == '?')
        {
            n_sentences++;
        }
        i++;
    }
    while (c != '\0');
    return n_sentences;
}

void compute_grade(string text)
{
    // Although these functions are nicely split, functional and readable; it would have been much more
    // efficient to just loop through the text once and determine all three numbers directly.
    int n_letters = count_letters(text);
    int n_words = count_words(text);
    int n_sentences = count_sentences(text);

    float l = (n_letters / (float) n_words) * 100;
    float s = (n_sentences / (float) n_words) * 100;

    float index = 0.0588 * l - 0.296 * s - 15.8;

    // Leave the index with decimal before checking the bound. Round it to integer before printing it.
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }

    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}