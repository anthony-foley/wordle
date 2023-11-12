#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_LINES 50
#define MAX_GUESSES 6
#define GREEN "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET "\e[0;39m"

void get_guess(char *input, int guesses);
int check_guess(char *input, char *word, int *correct_letters);
void print_guess(char *guess, int *correct_letters);
int check_game_end(int *correct_letters);

int main (int argc, char **argv)
{
    if (argc != 1)
    {
        printf("only enter one command line argument");
        return 1;
    }

    srand(time(NULL));

    int lineNumber = (rand() % NUM_LINES) + 1;

    FILE *words = fopen("words.txt", "r");
    if (words == NULL)
    {
        printf("error opening file\n");
        return 1;
    }

    char word[6];
    char buffer[100];

    for (int i = 0; i < lineNumber; i++) 
    {
        fgets(buffer, sizeof(buffer), words);
    }

    for (int j = 0; j < 5; j++)
    {
        word[j] = buffer[j];
    }
    word[5] = '\0';


    int guesses = 0;
    int has_won = 0;
    

    printf("You have %i attempts to guess the word\n", MAX_GUESSES);

    char *guess = (char *)calloc(6, sizeof(char));

    for (guesses = 0; guesses < MAX_GUESSES; guesses++)
    {
        get_guess(guess, guesses);
        guess = strlwr(guess);

        int correct_letters[5] = {0};

        check_guess(guess, word, correct_letters);

        print_guess(guess, correct_letters);

        if (check_game_end(correct_letters) == 10)
        {
            has_won = 1;
            break;
        }



    }

    if (has_won == 1)
    {
        printf("Congratulations, you have guessed the word!");
    }
    else 
    {
        printf("The word was %s\n", word);
    }

    return 0;
}

void get_guess(char *input, int guesses )
{
    do
    {
        printf("guess %i: ", guesses+1);
        fgets(input, 100, stdin);
    } while (strlen(input) != 6);

    input[5] = '\0';

}

int check_guess(char *input, char *word, int *correct_letters)
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (input[i] == word[j] && i == j)
            {
                correct_letters[i] = 2;
                break;
            }
            else if (input[i] == word[j])
            {
                correct_letters[i] = 1;
                //exhaust all letters --> needed for when theres a letter that appears more than once
                for (int k = j+1; k < 5; k++)
                {
                    if (input[i] == word[k] && i == k)
                    {
                        correct_letters[i] = 2;
                        break;
                    }
                    else if (input[i] == word[k])
                    {
                        correct_letters[i] = 1;
                        break;
                    }   
                    else if (input[i] == word[j] && correct_letters[j] == 2)
                    {
                        correct_letters[i] = 0;
                    }
                }
            
                break;
            }
            else 
            {
                correct_letters[i] = 0;
            }
        }
    }
}

void print_guess(char *guess, int *correct_letters)
{
    for (int i = 0; i < 5; i++)
    {
        if (correct_letters[i] == 0)
        {
            printf(RED "%c" RESET, guess[i]);
        }
        else if (correct_letters[i] == 1)
        {
            printf(YELLOW "%c" RESET, guess[i]);
        }
        else if (correct_letters[i] == 2)
        {
            printf(GREEN "%c" RESET, guess[i]);
        }
    }
    printf("\n");
}

int check_game_end(int *correct_letters)
{
    int sum = 0;

    for (int i = 0; i < 5; i++)
    {
        sum += correct_letters[i];
    }
    
    return sum;
}