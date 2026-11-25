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

int main(void)
{
    int stack[100];
    int top = 0;

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

            int b = pop(stack, &top);
            int a = pop(stack, &top);

            int res;

            switch (token[0])
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
        }
        else
        {
            int number = atoi(token);
            push(stack, &top, number);
        }
        token = strtok(NULL, " \n");
    }

    printf("Result of calculation: %d\n", stack[top - 1]);

    return 0;
}