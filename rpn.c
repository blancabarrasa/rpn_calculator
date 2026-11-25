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

enum TermType
{
    NUMBER,
    OPERATION,
    SKIP,
};

// representation of terms
struct Term
{
    enum TermType type;
    int value;
    char operator;
    struct Term *next;
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

int main(void)
{
    struct Term *termStack[100];
    int termTop = 0;
    int stack[100];
    int top = 0;

    struct Term *head = NULL;
    struct Term *tail = NULL;

    char input[100];

    printf("Enter an RPN expression: ");
    fgets(input, sizeof(input), stdin);

    char *token = strtok(input, " \n");

    while (token != NULL)
    {
        if (strcmp(token, "+") == 0 ||
            strcmp(token, "-") == 0 ||
            strcmp(token, "*") == 0 ||
            strcmp(token, "/") == 0)
        {
            struct Term *newTerm = malloc(sizeof(struct Term));
            if (newTerm == NULL)
            {
                printf("Error, null pointer");
                return 1;
            }
            else
            {
                newTerm->type = OPERATION;
                newTerm->operator = token[0];
                newTerm->next = NULL;
                if (head == NULL)
                {
                    head = tail = newTerm;
                }
                else
                {
                    tail->next = newTerm;
                    tail = newTerm;
                }
            }
        }
        else if (strcmp(token, "SKIP") == 0)
        {
            struct Term *newTerm = malloc(sizeof(struct Term));
            if (newTerm == NULL)
            {
                printf("Error, null pointer");
                return 1;
            }
            else
            {
                newTerm->type = SKIP;
                newTerm->next = NULL;
                if (head == NULL)
                {
                    head = tail = newTerm;
                }
                else
                {
                    tail->next = newTerm;
                    tail = newTerm;
                }
            }
        }
        else
        {
            int number = atoi(token);
            struct Term *newTerm = malloc(sizeof(struct Term));
            if (newTerm == NULL)
            {
                printf("Error, null pointer");
                return 1;
            }
            else
            {
                newTerm->type = NUMBER;
                newTerm->value = number;
                newTerm->next = NULL;
                if (head == NULL)
                {
                    head = tail = newTerm;
                }
                else
                {
                    tail->next = newTerm;
                    tail = newTerm;
                }
            }
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

        if (node->next != NULL)
        {
            pushTerm(termStack, &termTop, node->next);
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

    node = head;

    while (node != NULL)
    {
        struct Term *nextPtr = node->next;

        free(node);
        node = nextPtr;
    }

    printf("Result: %d\n", stack[top - 1]);

    return 0;
}
