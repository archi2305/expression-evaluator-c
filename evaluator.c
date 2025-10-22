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
typedef struct {
    char expressions[10][100];
    int front, rear, count;
} Queue;

void initQueue(Queue* q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

void enqueue(Queue* q, char* expr) {
    if (q->count == 10) {
        q->front = (q->front + 1) % 10; // overwrite oldest
        q->count--;
    }
    q->rear = (q->rear + 1) % 10;
    strcpy(q->expressions[q->rear], expr);
    q->count++;
}

void showHistory(Queue* q) {
    printf("\nRecent Calculations:\n");
    if (q->count == 0) {
        printf("No history available.\n");
        return;
    }
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % 10;
        printf("%d. %s\n", i+1, q->expressions[idx]);
    }
}

void infixToPostfix(char* infix, char* postfix) {
    Stack s;
    init(&s);
    int k = 0;
    int i = 0;

    while (infix[i] != '\0') {
        char c = infix[i];

        if (isdigit(c)) {
            // Extract full number
            while (isdigit(infix[i])) {
                postfix[k++] = infix[i++];
            }
            postfix[k++] = ' '; // space to separate numbers
            i--;
        }
        else if (c == '(') {
            push(&s, c);
        }
        else if (c == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[k++] = pop(&s);
                postfix[k++] = ' ';
            }
            pop(&s);
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!isEmpty(&s) && precedence(peek(&s)) >= precedence(c)) {
                postfix[k++] = pop(&s);
                postfix[k++] = ' ';
            }
            push(&s, c);
        }
        i++;
    }

    while (!isEmpty(&s)) {
        postfix[k++] = pop(&s);
        postfix[k++] = ' ';
    }

    postfix[k] = '\0';
}

int evaluatePostfix(char* postfix) {
    Stack s;
    init(&s);
    int i = 0;

    while (postfix[i] != '\0') {
        if (isdigit(postfix[i])) {
            int num = 0;
            while (isdigit(postfix[i])) {
                num = num * 10 + (postfix[i] - '0');
                i++;
            }
            push(&s, num);
        }
        else if (postfix[i] == '+' || postfix[i] == '-' || 
                 postfix[i] == '*' || postfix[i] == '/') {

            int val2 = pop(&s);
            int val1 = pop(&s);

            switch (postfix[i]) {
                case '+': push(&s, val1 + val2); break;
                case '-': push(&s, val1 - val2); break;
                case '*': push(&s, val1 * val2); break;
                case '/': 
                    if (val2 == 0) {
                        printf("Error: Division by zero!\n");
                        exit(1);
                    }
                    push(&s, val1 / val2); 
                    break;
            }
        }
        i++;
    }

    return pop(&s);
}
int isValidChar(char c) {
    // Allow digits, operators, parentheses
    return (isdigit(c) || c=='+' || c=='-' || c=='*' || c=='/' || c=='(' || c==')');
}

int areParenthesesBalanced(char* exp) {
    Stack s;
    init(&s);
    for (int i = 0; i < strlen(exp); i++) {
        if (exp[i] == '(')
            push(&s, '(');
        else if (exp[i] == ')') {
            if (isEmpty(&s))
                return 0;  // more ')' than '('
            pop(&s);
        }
    }
    return isEmpty(&s);  // should be empty if balanced
}


int main() {
    char infix[100], postfix[100];
    Queue history;
    initQueue(&history);
    int choice = 1;

    while (choice) {
        printf("\nEnter infix expression (e.g., 3+4*2): ");
        scanf("%s", infix);

        for (int i = 0; i < strlen(infix); i++) {
            if (!isValidChar(infix[i])) {
                printf("Error: Invalid input!\n");
                exit(1);
            }
        }

        infixToPostfix(infix, postfix);
        printf("Postfix expression: %s\n", postfix);

        int result = evaluatePostfix(postfix);
        printf("Result: %d\n", result);

        char record[100];
        sprintf(record, "%s = %d", infix, result);
        enqueue(&history, record);

        printf("\nDo you want to see history (1=Yes, 0=No)? ");
        int view;
        scanf("%d", &view);
        if (view == 1) showHistory(&history);

        printf("\nDo you want to continue (1=Yes, 0=No)? ");
        scanf("%d", &choice);
    }

    return 0;
}
