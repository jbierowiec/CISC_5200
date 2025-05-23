#include <stdio.h>
#include <string.h>

#define MAX_PATHS 100
#define MAX_LEN 100

#define Q0 0
#define Q2 2 // Even 0s (ACCEPTING)
#define Q3 3 // Odd 0s (REJECTING)

typedef struct
{
    int state;
    char trace[MAX_LEN];
    int accepted;
    int path_id;
} Path;

void print_state(int s)
{
    if (s == Q0)
        printf("q0");
    else if (s == Q2)
        printf("q2 (ACCEPTING)");
    else if (s == Q3)
        printf("q3 (REJECTING)");
}

int step_paths(Path *paths, int size, char input, Path *next_paths, int *next_id_counter)
{
    int next_size = 0;
    printf("Input '%c': \n", input);

    for (int i = 0; i < size; i++)
    {
        Path p = paths[i];
        int prev_state = p.state;

        printf("Path %d: ", p.path_id);
        print_state(p.state);
        printf(" on '%c' -> ", input);

        if (p.state == Q2)
        {
            if (input == '0')
            {
                Path fork = p;
                fork.state = Q3;
                sprintf(fork.trace + strlen(fork.trace), "q2 -> q3 ");
                fork.path_id = (*next_id_counter)++;
                next_paths[next_size++] = fork;
                printf("fork to Path %d: q3\n", fork.path_id);
                if (prev_state == Q2 && fork.state == Q3)
                {
                    printf("Transitioned from ACCEPTING to REJECTING state.\n");
                }
            }
            else if (input == '1')
            {
                p.state = Q2;
                sprintf(p.trace + strlen(p.trace), "q2 -> q2 ");
                next_paths[next_size++] = p;
                printf("q2\n");
            }
        }
        else if (p.state == Q3)
        {
            if (input == '0')
            {
                Path fork = p;
                fork.state = Q2;
                sprintf(fork.trace + strlen(fork.trace), "q3 -> q2 ");
                fork.path_id = (*next_id_counter)++;
                next_paths[next_size++] = fork;
                printf("fork to Path %d: q2\n", fork.path_id);
                if (prev_state == Q3 && fork.state == Q2)
                {
                    printf("Transitioned from REJECTING to ACCEPTING state.\n");
                }
            }
            else if (input == '1')
            {
                p.state = Q3;
                sprintf(p.trace + strlen(p.trace), "q3 -> q3 ");
                next_paths[next_size++] = p;
                printf("q3\n");
            }
        }
        else
        {
            printf("{}");
        }
    }

    return next_size;
}

int main()
{
    char input[MAX_LEN];
    Path paths[MAX_PATHS], next_paths[MAX_PATHS];
    int path_count = 0, next_id_counter = 1;

    printf("\n");
    printf("Enter a binary string (only 0s and 1s): ");
    scanf("%s", input);

    printf("\n--- NFA State Trace With Parallel Branching ---\n");

    // ε-transitions from q0 → q2 and q0 → q3 (2 parallel paths)
    paths[0].state = Q2;
    strcpy(paths[0].trace, "q0 -> q2 ");
    paths[0].path_id = next_id_counter++;
    paths[0].accepted = 0;

    paths[1].state = Q3;
    strcpy(paths[1].trace, "q0 -> q3 ");
    paths[1].path_id = next_id_counter++;
    paths[1].accepted = 0;

    path_count = 2;

    printf("Start at q0 (ACCEPTING) \n");

    for (int i = 0; i < strlen(input); i++)
    {
        char c = input[i];
        path_count = step_paths(paths, path_count, c, next_paths, &next_id_counter);

        for (int j = 0; j < path_count; j++)
        {
            paths[j] = next_paths[j];
        }

        printf("\n");
    }

    printf("--- Final Paths ---\n");
    for (int i = 0; i < path_count; i++)
    {
        Path *p = &paths[i];
        p->accepted = (p->state == Q2);

        printf("Path %d:\n", p->path_id);
        printf("%s\n", p->trace);
        printf("Final state: ");
        print_state(p->state);
        printf("\n\n");
    }

    printf("--- Result ---");
    printf("\n");
    for (int i = 0; i < path_count; i++)
    {
        Path *p = &paths[i];
        p->accepted = (p->state == Q2);

        if (p->accepted)
        {
            printf("String accepted: Even number of 0s.\n");
        }
        else
        {
            printf("String rejected: Odd number of 0s.\n");
        }
    }

    printf("\n");

    return 0;
}
