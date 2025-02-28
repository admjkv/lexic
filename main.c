#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_GUESSES 6

int main(void)
{
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // List of words to guess
    const char *words[] = {"apple", "banana", "cherry", "date", "elderberry"};
    size_t num_words = sizeof(words) / sizeof(words[0]);

    // Select a random word from the list
    const char *target = words[rand() % num_words];
    size_t target_length = strlen(target);

    // Array to track which letters have been guessed correctly
    bool guessed[target_length];
    for (size_t i = 0; i < target_length; i++)
    {
        guessed[i] = false;
    }

    bool used_letters[26] = {false}; // Track used letters (a-z)

    int incorrect_guesses = 0;
    char guess;
    bool word_complete = false;

    // Game loop
    while (incorrect_guesses < MAX_GUESSES && !word_complete)
    {
        // Display the current progress
        printf("Word: ");
        for (size_t i = 0; i < target_length; i++)
        {
            if (guessed[i])
                printf("%c", target[i]);
            else
                printf("*");
        }
        printf("\n");

        printf("Used letters: ");
        for (char c = 'a'; c <= 'z'; c++) {
            if (used_letters[c - 'a']) {
                printf("%c ", c);
            }
        }
        printf("\n");

        // Prompt user for a letter
        printf("Enter a letter: ");
        scanf(" %1c", &guess);

        if (used_letters[guess - 'a']) {
            printf("You already guessed '%c'. Try another letter.\n", guess);
            continue;
        } else {
            used_letters[guess - 'a'] = true;
        }

        // Check if the guessed letter is in the word
        bool correct = false;
        for (size_t i = 0; i < target_length; i++)
        {
            if (target[i] == guess)
            {
                guessed[i] = true;
                correct = true;
            }
        }

        if (!correct)
        {
            incorrect_guesses++;
            printf("Incorrect guess! You have %d guesses left.\n", MAX_GUESSES - incorrect_guesses);
        }

        // Check if the whole word is guessed
        word_complete = true;
        for (size_t i = 0; i < target_length; i++)
        {
            if (!guessed[i])
            {
                word_complete = false;
                break;
            }
        }
    }

    if (word_complete)
        printf("Congratulations! You guessed the word: %s\n", target);
    else
        printf("Game over! The word was: %s\n", target);

    return 0;
}
