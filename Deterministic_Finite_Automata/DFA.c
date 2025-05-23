#include <stdio.h>
#include <string.h>

// Define state constants
#define Q0 0 // q0: Accepting state
#define Q1 1 // q1: Rejecting state

// Function to convert state int to readable string
const char *get_state_label(int state)
{
    if (state == Q0)
    {
        return "q0";
    }
    else
    {
        return "q1";
    }
}

// Function to convert state int to readable string
const char *get_state_name(int state)
{
    if (state == Q0)
    {
        return "q0 (ACCEPTING)";
    }
    else
    {
        return "q1 (REJECTING)";
    }
}

// Transition function
int transition(int current_state, char input_char)
{
    if (current_state == Q0)
    {
        if (input_char == '0')
            return Q1;
        else
            return Q0;
    }
    else
    {
        if (input_char == '0')
            return Q0;
        else
            return Q1;
    }
}

int main()
{
    char input[100];
    char trace[1000] = "";
    int state = Q0;

    printf("\n");
    printf("Enter a binary string (only 0s and 1s): ");
    scanf("%s", input);

    printf("\n--- DFA State Trace ---\n");
    printf("Start at %s\n", get_state_name(state));

    for (int i = 0; i < strlen(input); i++)
    {
        char current = input[i];

        if (current != '0' && current != '1')
        {
            printf("Invalid character '%c'. Only 0s and 1s are allowed.\n", current);
            return 1;
        }

        int prev_state = state;
        state = transition(state, current);
        char step[50];

        sprintf(step, "%s -> %s ", get_state_label(prev_state), get_state_label(state));
        strcat(trace, step);

        printf("Input '%c':\n", current);
        printf("%s on '%c' -> %s\n", get_state_name(prev_state), current, get_state_label(state));

        if (prev_state == Q0 && state == Q1)
        {
            printf("Transitioned from ACCEPTING to REJECTING state.\n");
        }
        else if (prev_state == Q1 && state == Q0)
        {
            printf("Transitioned from REJECTING to ACCEPTING state.\n");
        }

        printf("\n");
    }

    printf("--- Final Path ---\n");
    printf("%s\n", trace);
    printf("Final state: %s\n", get_state_name(state));

    printf("\n--- Result ---\n");
    if (state == Q0)
    {
        printf("String accepted: Even number of 0s.\n\n");
    }
    else
    {
        printf("String rejected: Odd number of 0s.\n\n");
    }

    return 0;
}
