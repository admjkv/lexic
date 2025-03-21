#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_GUESSES 6

// Add these definitions at the top
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define RESET   "\x1b[0m"

#define MAX_HINTS 3

// Word lists moved to global scope
const char *easy_words[] = {"apple", "banana", "cherry", "date", "fig", "grape", "kiwi", "lemon", 
                           "mango", "melon", "olive", "orange", "peach", "pear", "plum"};
const char *medium_words[] = {"apricot", "blueberry", "coconut", "dragonfruit", "guava", "kumquat", 
                             "papaya", "pineapple", "raspberry", "strawberry", "tangerine", "watermelon"};
const char *hard_words[] = {"elderberry", "jackfruit", "persimmon", "rambutan", "blackberry", 
                           "blackcurrant", "boysenberry", "gooseberry", "lychee", "mulberry", "pomegranate"};

// Function to display hangman ASCII art
void display_hangman(int incorrect_guesses) {
    printf("\n");
    switch (incorrect_guesses) {
        case 0:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 1:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 2:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf("  |   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 3:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 4:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|\\  |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 5:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|\\  |\n");
            printf(" /    |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 6:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|\\  |\n");
            printf(" / \\  |\n");
            printf("      |\n");
            printf("=========\n");
            break;
    }
    printf("\n");
}

// Function to get difficulty level from user
int get_difficulty() {
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
        
        // Clear input buffer
        while (getchar() != '\n');
        
        if (difficulty >= 1 && difficulty <= 3) {
            valid_input = 1;
        } else {
            printf("Please enter a number between 1 and 3.\n\n");
        }
    }
    
    return difficulty;
}

// Function to select target word based on difficulty
const char* select_word(int difficulty) {
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
    return target;
}

// Function to play one round of the game
bool play_game() {
    int difficulty = get_difficulty();
    const char *target = select_word(difficulty);
    size_t target_length = strlen(target);

    // Array to track which letters have been guessed correctly
    bool guessed[target_length];
    for (size_t i = 0; i < target_length; i++) {
        guessed[i] = false;
    }

    bool used_letters[26] = {false}; // Track used letters (a-z)
    int incorrect_guesses = 0;
    bool word_complete = false;

    int hints_remaining = MAX_HINTS;

    system("clear"); // Start with a clean screen

    // Game loop
    while (incorrect_guesses < MAX_GUESSES && !word_complete) {
        // Display hangman
        display_hangman(incorrect_guesses);
        
        // Display the current progress
        printf(YELLOW "Word: " RESET);
        for (size_t i = 0; i < target_length; i++) {
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

        printf("Incorrect guesses: %d/%d\n", incorrect_guesses, MAX_GUESSES);
        printf("Hints remaining: %d\n", hints_remaining);

        // Prompt user for a letter
        printf(BLUE "Enter a letter (or '!' to quit or '?' for a hint): " RESET);
        char guess;
        scanf(" %c", &guess);
        while (getchar() != '\n'); // Clear input buffer

        if (guess == '!') {
            printf("Game aborted. The word was: %s\n", target);
            return false;
        }

        if (guess == '?' && hints_remaining > 0) {
            // Provide a hint by revealing a random unguessed letter
            int unguessed_count = 0;
            for (size_t i = 0; i < target_length; i++) {
                if (!guessed[i]) unguessed_count++;
            }
            
            if (unguessed_count > 0) {
                int hint_index = rand() % unguessed_count;
                int current = 0;
                
                for (size_t i = 0; i < target_length; i++) {
                    if (!guessed[i]) {
                        if (current == hint_index) {
                            guessed[i] = true;
                            printf("Hint: The word contains the letter '%c'.\n", target[i]);
                            hints_remaining--;
                            break;
                        }
                        current++;
                    }
                }
            }
            continue;
        }

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
        for (size_t i = 0; i < target_length; i++) {
            if (target[i] == guess) {
                guessed[i] = true;
                correct = true;
            }
        }

        if (!correct) {
            incorrect_guesses++;
            printf("Incorrect guess! You have %d guesses left.\n", MAX_GUESSES - incorrect_guesses);
        }

        // Check if the whole word is guessed
        word_complete = true;
        for (size_t i = 0; i < target_length; i++) {
            if (!guessed[i]) {
                word_complete = false;
                break;
            }
        }
        
        system("clear"); // Clear screen for next round
    }

    // Final display
    display_hangman(incorrect_guesses);
    
    if (word_complete)
        printf(GREEN "Congratulations! You guessed the word: %s\n" RESET, target);
    else
        printf(RED "Game over! The word was: %s\n" RESET, target);
        
    return true;
}

int main(void) {
    // Seed the random number generator
    srand((unsigned int)time(NULL));
    
    bool play_again = true;
    
    printf("Welcome to Lexic - A Word Guessing Game!\n\n");
    
    while (play_again) {
        play_game();
        
        char choice;
        printf("Would you like to play again? (y/n): ");
        scanf(" %c", &choice);
        while (getchar() != '\n'); // Clear input buffer
        
        play_again = (choice == 'y' || choice == 'Y');
        
        if (play_again) {
            system("clear");
        }
    }
    
    printf("Thanks for playing!\n");
    return 0;
}
