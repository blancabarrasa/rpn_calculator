#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void push(int stack[], int *top, int value)
{
    stack[*top] = value;
    (*top)++;
}

int pop(int stack[], int *top)
{
    (*top)--;
    int popped = stack[*top];
    return popped;
}

// Introducing sequencing and skip
enum TermType
{
    NUMBER,
    OPERATION,
    SKIP,
    SEQUENCE,
};

// representation of terms
struct Term
{
    enum TermType type;
    int value;
    char operator;
    struct Term *left;
    struct Term *right;
};

void pushTerm(struct Term *termStack[], int *termTop, struct Term *term)
{
    termStack[*termTop] = term;
    (*termTop)++;
};

struct Term *popTerm(struct Term *termStack[], int *termTop)
{
    (*termTop)--;
    struct Term *popped = termStack[*termTop];
    return popped;
}

void freeTerm(struct Term *termPtr)
{
    if (termPtr == NULL)
    {
        return;
    }
    freeTerm(termPtr->left);
    freeTerm(termPtr->right);
    free(termPtr);
}

int main(void)
{
    // Continuation stack
    struct Term *termStack[100];
    int termTop = 0;

    // Integer stack
    int stack[100];
    int top = 0;

    struct Term *head = NULL;

    char input[100];

    printf("Enter an RPN expression: ");
    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, " \n");

    while (token != NULL)
    {
        struct Term *newTerm = malloc(sizeof(struct Term));
        if (newTerm == NULL)
        {
            printf("Error, null pointer");
            return 1;
        }
        newTerm->left = NULL;
        newTerm->right = NULL;
        if (strcmp(token, "+") == 0 ||
            strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 ||
            strcmp(token, "/") == 0)
        {
            newTerm->type = OPERATION;
            newTerm->operator = token[0];
        }
        else if (strcmp(token, "SKIP") == 0)
        {

            newTerm->type = SKIP;
        }
        else
        {
            int number = atoi(token);

            newTerm->type = NUMBER;
            newTerm->value = number;
        }
        if (head == NULL)
        {
            head = newTerm;
        }
        else
        {
            struct Term *sequence = malloc(sizeof(struct Term));
            if (sequence == NULL)
            {
                printf("Error, null pointer");
                return 1;
            }
            sequence->type = SEQUENCE;
            sequence->left = head;
            sequence->right = newTerm;
            head = sequence;
        }

        token = strtok(NULL, " \n");
    }

    if (head == NULL)
    {
        printf("No expression entered\n");
        return 1;
    }

    pushTerm(termStack, &termTop, head);
    struct Term *node = popTerm(termStack, &termTop);

    while (node != NULL)
    {
        if (node->type == NUMBER)
        {
            push(stack, &top, node->value);
            printf("NUMBER: %d\n", node->value);
        }
        if (node->type == OPERATION)
        {
            int b = pop(stack, &top);
            int a = pop(stack, &top);

            int res;

            switch (node->operator)
            {
            case '+':
                res = a + b;
                break;
            case '-':
                res = a - b;
                break;
            case '*':
                res = a * b;
                break;
            case '/':
                res = a / b;
                break;
            default:
                printf("Unknown operator!\n");
                break;
            }
            push(stack, &top, res);
            printf("OPERATION: %c\n", node->operator);
        }
        if (node->type == SKIP)
        {
            printf("SKIP\n");
        }
        if (node->type == SEQUENCE)
        {
            pushTerm(termStack, &termTop, node->right);
            pushTerm(termStack, &termTop, node->left);
        }

        if (termTop > 0)
        {
            node = popTerm(termStack, &termTop);
        }
        else
        {
            node = NULL;
        }
    }

    freeTerm(head);

    printf("Result: %d\n", stack[top - 1]);

    return 0;
}
