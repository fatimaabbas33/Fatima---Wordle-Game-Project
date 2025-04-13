#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//ANSI color codes for terminal output to colorize feedback
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define RED "\x1B[31m"
#define RESET "\x1B[0m"

//function to check if a string contains only alphabetic characters + matches the expected word length
bool isValidWord(const char *word, int wordLength) {
    if (strlen(word) != wordLength) return false;
    for (int i = 0; i < wordLength; i++) {
        if (!isalpha(word[i])) return false;
    }
    return true;
}

//function to convert a string to lowercase
void toLowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

//function to evaluate the guess and provide colored feedback
void evaluateGuess(const char *target, const char *guess, int wordLength) {
    bool usedTarget[wordLength]; //array that tracks letters in the target word that have been matched
    char colors[wordLength][20]; //array that stores the color codes for each letter

    //initialize arrays (All letters are unused and the defaut color is red)
    memset(usedTarget, false, sizeof(usedTarget));
    for (int i = 0; i < wordLength; i++) strcpy(colors[i], RED);

    //first pass: Mark correct letters in the correct position (green)
    for (int i = 0; i < wordLength; i++) {
        if (guess[i] == target[i]) {
            strcpy(colors[i], GREEN); //green for correct letter and position
            usedTarget[i] = true;
        }
    }

    //second pass: mark placeholders to yellow for correct letters in wrong positions
    for (int i = 0; i < wordLength; i++) {
        if (strcmp(colors[i], RED) == 0) { //to skip already green letters
            for (int j = 0; j < wordLength; j++) {
                if (!usedTarget[j] && guess[i] == target[j]) {
                    strcpy(colors[i], YELLOW); //change colour to yellow
                    usedTarget[j] = true;
                    break;
                }
            }
        }
    }

    // Print the feedback with colors
    printf("Feedback: ");
    for (int i = 0; i < wordLength; i++) {
        printf("%s%c" RESET, colors[i], guess[i]);
    }
    printf("\n");
}

int main() {
    //list of 30 possible target words categorized by length
    const char *wordList4[] = {"cake", "tree", "love", "moon", "star", "cash", "sand", "face", "book", "cold"};
    const char *wordList5[] = {"apple", "grape", "lemon", "peach", "berry", "table", "young", "alive", "early", "night"};
    const char *wordList6[] = {"banana", "orange", "cherry", "ignore", "grapes", "breeze", "circle", "dimple", "ballet", "violin"};

    int wordLength, maxAttempts = 6; //maximum number of attempts allowed
    int wordCount;

    //prompt user to select word length
    printf("Welcome to Wordle!\n");
    printf("Choose the word length:\n");
    printf("1. 4 letters\n");
    printf("2. 5 letters\n");
    printf("3. 6 letters\n");
    printf("Enter your choice (1/2/3): ");
    int choice;
    scanf("%d", &choice);
    getchar(); //consume newline character

    //set word length and count based on user's choice
    switch (choice) {
        case 1:
            wordLength = 4;
            wordCount = sizeof(wordList4) / sizeof(wordList4[0]);
            break;
        case 2:
            wordLength = 5;
            wordCount = sizeof(wordList5) / sizeof(wordList5[0]);
            break;
        case 3:
            wordLength = 6;
            wordCount = sizeof(wordList6) / sizeof(wordList6[0]);
            break;
        default:
            printf("Invalid choice. Defaulting to 5-letter words.\n");
            wordLength = 5;
            wordCount = sizeof(wordList5) / sizeof(wordList5[0]);
    }

    //select a random target word from the chosen list
    srand(time(NULL));
    const char *targetWord;
    switch (wordLength) {
        case 4:
            targetWord = wordList4[rand() % wordCount];
            break;
        case 5:
            targetWord = wordList5[rand() % wordCount];
            break;
        case 6:
            targetWord = wordList6[rand() % wordCount];
            break;
    }

    char guess[wordLength + 1]; //buffer to hold the player's guess 
    int attempts = 0;

    //display game instructions and rules
    printf("\nYou have %d attempts to guess the %d-letter word.\n", maxAttempts, wordLength);
    printf("Feedback Colors:\n");
    printf(GREEN "Green" RESET " - Correct letter and position\n");
    printf(YELLOW "Yellow" RESET " - Correct letter, wrong position\n");
    printf(RED "Red" RESET " - Incorrect letter\n");

    //start the stopwatch
    clock_t start = clock();

    while (attempts < maxAttempts) {
        printf("\nAttempt %d/%d: Enter your guess: ", attempts + 1, maxAttempts);
        scanf("%s", guess);
        getchar(); //consume the newline character by scanf

        //validate the input
        if (!isValidWord(guess, wordLength)) {
            printf("Invalid input! Please enter a %d-letter word containing only alphabetic characters.\n", wordLength);
            continue;
        }

        //convert guess to lowercase
        toLowercase(guess);

        //evaluate the guess and display feedback
        evaluateGuess(targetWord, guess, wordLength);

        //check to see if the guess is correct
        if (strcmp(guess, targetWord) == 0) {
            clock_t end = clock(); //stop the stopwatch
            double timeSpent = (double)(end - start) / CLOCKS_PER_SEC; //calculate elapsed time
            printf("Congratulations! You guessed the word correctly in %d attempt(s).\n", attempts + 1);
            printf("Time taken: %.2f seconds.\n", timeSpent);
            return 0; //exit the program after a successfull guess
        }

        attempts++; //increment attempt counter
    }

    //if player runs out of attempts, reveal the target word
    printf("\nGame over! The word was '%s'. Better luck next time!\n", targetWord);

    return 0;
}