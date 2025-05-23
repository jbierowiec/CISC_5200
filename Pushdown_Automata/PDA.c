#include <stdio.h>
#include <string.h>

#define MAX_STACK 100

// Stack data structure
typedef struct {
    char data[MAX_STACK];
    int top;
} Stack;

void init(Stack *s) {
    s->top = -1;
}

int is_empty(Stack *s) {
    return s->top == -1;
}

int push(Stack *s, char c) {
    if (s->top < MAX_STACK - 1) {
        s->data[++(s->top)] = c;
        return 1;
    }
    return 0;
}

char pop(Stack *s) {
    if (!is_empty(s)) {
        return s->data[(s->top)--];
    }
    return '\0';
}

char peek(Stack *s) {
    return is_empty(s) ? '\0' : s->data[s->top];
}

int main() {
    char input[100];
    Stack stack;
    init(&stack);

    printf("\n");
    printf("Enter a binary string (only 0s and 1s): ");
    scanf("%s", input);

    printf("\n--- PDA Trace ---\n");

    // Initial stack symbol
    push(&stack, 'Z');
    printf("Initial stack: Z (Z = bottom marker).\n\n");

    for (int i = 0; i < strlen(input); i++) {
        char c = input[i];

        if (c != '0' && c != '1') {
            printf("Invalid character '%c'. Only 0s and 1s are allowed.\n", c);
            return 1;
        }

        printf("Read '%c': ", c);

        if (c == '0') {
            // Toggle X on stack
            if (peek(&stack) == 'X') {
                pop(&stack);
                printf("Popped X -> even 0s so far.\n");
            } else {
                push(&stack, 'X');
                printf("Pushed X -> odd 0s so far.\n");
            }
        } else {
            printf("Ignored (1 has no effect on stack).\n");
        }
    }

    // Acceptance condition: only Z remains
    printf("\nFinal stack: ");
    for (int i = 0; i <= stack.top; i++) {
        printf("%c", stack.data[i]);
    }
    printf("\n");

    printf("\n--- Result ---\n");
    if (stack.top == 0 && peek(&stack) == 'Z') {
        printf("String accepted: Even number of 0s.\n");
    } else {
        printf("String rejected: Odd number of 0s.\n");
    }
    printf("\n");

    return 0;
}
