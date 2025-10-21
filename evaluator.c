#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isdigit()

#define MAX 100

typedef struct {
    char items[MAX];
    int top;
} Stack;
void init(Stack* s) {
    s->top = -1;
}
void push(Stack* s, char c) {
    if (s->top == MAX - 1) {
        printf("Stack Overflow\n");
        return;
    }
    s->items[++(s->top)] = c;
}

char pop(Stack* s) {
    if (s->top == -1) {
        return '\0'; // empty stack
    }
    return s->items[(s->top)--];
}

char peek(Stack* s) {
    if (s->top == -1) return '\0';
    return s->items[s->top];
}

int isEmpty(Stack* s) {
    return s->top == -1;
}
int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
    }
    return 0;
}
void infixToPostfix(char* infix, char* postfix) {
    Stack s;
    init(&s);
    int k = 0;

    for (int i = 0; i < strlen(infix); i++) {
        char c = infix[i];

        if (isdigit(c)) { // Operand
            postfix[k++] = c;
        } 
        else if (c == '(') {
            push(&s, c);
        } 
        else if (c == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[k++] = pop(&s);
            }
            pop(&s); // remove '('
        } 
        else { // Operator
            while (!isEmpty(&s) && precedence(peek(&s)) >= precedence(c)) {
                postfix[k++] = pop(&s);
            }
            push(&s, c);
        }
    }

    while (!isEmpty(&s)) {
        postfix[k++] = pop(&s);
    }

    postfix[k] = '\0'; // null-terminate string
}
int main() {
    char infix[100], postfix[100];

    printf("Enter infix expression (e.g., 3+4*2): ");
    scanf("%s", infix);

    infixToPostfix(infix, postfix);

    printf("Postfix expression: %s\n", postfix);

    return 0;
}
