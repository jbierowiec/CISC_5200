#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_SIZE 200
#define MAX_RULES 100
#define MAX_STATE_LEN 10
#define BLANK '_'
#define STEP_LIMIT 1000

typedef struct
{
    char current_state[MAX_STATE_LEN];
    char read_symbol;
    char write_symbol;
    char move_direction; // 'L' or 'R'
    char next_state[MAX_STATE_LEN];
} Rule;

Rule rules[MAX_RULES];
int rule_count = 0;

void load_sample_rules()
{
    rule_count = 0;

    // q0
    strcpy(rules[rule_count].current_state, "q0");
    rules[rule_count].read_symbol = '0';
    rules[rule_count].write_symbol = 'X';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q1");

    strcpy(rules[rule_count].current_state, "q0");
    rules[rule_count].read_symbol = '1';
    rules[rule_count].write_symbol = '1';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q0");

    strcpy(rules[rule_count].current_state, "q0");
    rules[rule_count].read_symbol = 'X';
    rules[rule_count].write_symbol = 'X';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q0");

    strcpy(rules[rule_count].current_state, "q0");
    rules[rule_count].read_symbol = '_';
    rules[rule_count].write_symbol = '_';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "qaccept");

    // q1
    strcpy(rules[rule_count].current_state, "q1");
    rules[rule_count].read_symbol = '0';
    rules[rule_count].write_symbol = 'X';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q2");

    strcpy(rules[rule_count].current_state, "q1");
    rules[rule_count].read_symbol = '1';
    rules[rule_count].write_symbol = '1';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q1");

    strcpy(rules[rule_count].current_state, "q1");
    rules[rule_count].read_symbol = 'X';
    rules[rule_count].write_symbol = 'X';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q1");

    strcpy(rules[rule_count].current_state, "q1");
    rules[rule_count].read_symbol = '_';
    rules[rule_count].write_symbol = '_';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "qreject");

    // q2
    strcpy(rules[rule_count].current_state, "q2");
    rules[rule_count].read_symbol = '0';
    rules[rule_count].write_symbol = '0';
    rules[rule_count].move_direction = 'L';
    strcpy(rules[rule_count++].next_state, "q3");

    strcpy(rules[rule_count].current_state, "q2");
    rules[rule_count].read_symbol = '1';
    rules[rule_count].write_symbol = '1';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q2");

    strcpy(rules[rule_count].current_state, "q2");
    rules[rule_count].read_symbol = 'X';
    rules[rule_count].write_symbol = 'X';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q2");

    strcpy(rules[rule_count].current_state, "q2");
    rules[rule_count].read_symbol = '_';
    rules[rule_count].write_symbol = '_';
    rules[rule_count].move_direction = 'L';
    strcpy(rules[rule_count++].next_state, "q3");

    // q3
    strcpy(rules[rule_count].current_state, "q3");
    rules[rule_count].read_symbol = '0';
    rules[rule_count].write_symbol = '0';
    rules[rule_count].move_direction = 'L';
    strcpy(rules[rule_count++].next_state, "q3");

    strcpy(rules[rule_count].current_state, "q3");
    rules[rule_count].read_symbol = '1';
    rules[rule_count].write_symbol = '1';
    rules[rule_count].move_direction = 'L';
    strcpy(rules[rule_count++].next_state, "q3");

    strcpy(rules[rule_count].current_state, "q3");
    rules[rule_count].read_symbol = 'X';
    rules[rule_count].write_symbol = 'X';
    rules[rule_count].move_direction = 'L';
    strcpy(rules[rule_count++].next_state, "q3");

    strcpy(rules[rule_count].current_state, "q3");
    rules[rule_count].read_symbol = '_';
    rules[rule_count].write_symbol = '_';
    rules[rule_count].move_direction = 'R';
    strcpy(rules[rule_count++].next_state, "q0");
}

Rule *find_rule(const char *state, char symbol)
{
    for (int i = 0; i < rule_count; i++)
    {
        if (strcmp(rules[i].current_state, state) == 0 && rules[i].read_symbol == symbol)
        {
            return &rules[i];
        }
    }
    return NULL;
}

void print_tape(char tape[], int head)
{
    int start = head - 50;
    int end = head + 49;

    if (start < 0)
        start = 0;
    if (end >= TAPE_SIZE)
        end = TAPE_SIZE - 1;

    for (int i = start; i <= end; i++)
    {
        printf("%c", tape[i]);
    }
    printf("\n");

    for (int i = start; i < head; i++)
    {
        printf(" ");
    }
    printf("^\n");
}

int is_binary_string(const char *s)
{
    for (int i = 0; s[i]; i++)
    {
        if (s[i] != '0' && s[i] != '1')
            return 0;
    }
    return 1;
}

int main()
{
    char input[100];
    char tape[TAPE_SIZE];
    int head = TAPE_SIZE / 2;
    char current_state[MAX_STATE_LEN] = "q0";
    int step = 0;

    printf("\nEnter a binary string (only 0s and 1s): ");
    scanf("%s", input);

    if (!is_binary_string(input))
    {
        printf("Invalid input. Only 0s and 1s allowed.\n");
        return 1;
    }

    for (int i = 0; i < TAPE_SIZE; i++)
    {
        tape[i] = BLANK;
    }
    for (int i = 0; i < strlen(input); i++)
    {
        tape[head + i] = input[i];
    }

    load_sample_rules();

    printf("\n--- Turing Machine Trace ---\n");

    while (step < STEP_LIMIT)
    {
        char symbol = tape[head];
        Rule *rule = find_rule(current_state, symbol);

        printf("Step %d: State: %s, Head: %d, Read: '%c'\n", step, current_state, head, symbol);
        print_tape(tape, head);
        getchar(); // Pause for Enter key

        if (!rule)
        {
            printf("No matching rule. Halting.\n");
            break;
        }

        tape[head] = rule->write_symbol;

        if (rule->move_direction == 'R')
        {
            head++;
        }
        else if (rule->move_direction == 'L')
        {
            head--;
        }

        if (strcmp(rule->next_state, "qaccept") == 0)
        {
            printf("Transition to qaccept.\n\nString accepted.\n");
            strcpy(current_state, "qaccept");
            break;
        }
        else if (strcmp(rule->next_state, "qreject") == 0)
        {
            printf("Transition to qreject.\n\nString rejected.\n");
            strcpy(current_state, "qreject");
            break;
        }
        else
        {
            strcpy(current_state, rule->next_state);
        }

        step++;
    }

    printf("\n--- Result ---\n");
    if (strcmp(current_state, "qaccept") == 0)
    {
        printf("String accepted: Even number of 0s.\n\n");
    }
    else if (strcmp(current_state, "qreject") == 0)
    {
        printf("String rejected: Odd number of 0s or no valid transition.\n\n");
    }
    else if (step >= STEP_LIMIT)
    {
        printf("Step limit reached. Potential infinite loop.\n\n");
    }
    else
    {
        printf("Machine halted unexpectedly.\n\n");
    }

    return 0;
}
