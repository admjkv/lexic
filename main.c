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

    // Different levels of difficulty has different complexity of words
    const char *easy_words[] = {"apple", "banana", "cherry", "date", "fig"};
    const char *medium_words[] = {"apricot", "blueberry", "coconut", "dragonfruit"};
    const char *hard_words[] = {"elderberry", "jackfruit", "persimmon", "rambutan"};

    // Input validation for difficulty level
    int difficulty;
    int valid_input = 0;
    
    while (!valid_input) {
        printf("Select difficulty level:\n");
        printf("1. Easy\n2. Medium\n3. Hard\n");
        printf("Enter your choice (1-3): ");
        
        if (scanf("%d", &difficulty) != 1) {
            // Clear input buffer if non-numeric input
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n\n");
            continue;
        }
        
        if (difficulty >= 1 && difficulty <= 3) {
            valid_input = 1;
        } else {
            printf("Please enter a number between 1 and 3.\n\n");
        }
    }

    // Select word by chosen difficulty
    const char *target;
    switch(difficulty) {
        case 3:
            target = hard_words[rand() % (sizeof(hard_words) / sizeof(hard_words[0]))];
            break;
        case 2:
            target = medium_words[rand() % (sizeof(medium_words) / sizeof(medium_words[0]))];
            break;
        default:
            target = easy_words[rand() % (sizeof(easy_words) / sizeof(easy_words[0]))];
    }

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
        while (getchar() != '\n'); // Clear input buffer

        // Validation for letter input
        if (guess < 'a' || guess > 'z') {
            printf("Please enter a lowercase letter (a-z).\n");
            continue;
        }

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

    char play_again;
    printf("Would you like to play again? (y/n): ");
    scanf(" %1c", &play_again);
    if (play_again == 'y' || play_again == 'Y') {
        // Clear screen for new game
        system("clear");
        main(); // Restart the game
    }

    return 0;
}
