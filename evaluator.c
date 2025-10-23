#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// 🎨 COLORS
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"
#define BOLD    "\x1b[1m"

#define MAX 100

// ===================== CHAR STACK (for infix→postfix) =====================
typedef struct {
    char items[MAX];
    int top;
} CharStack;

void initChar(CharStack* s) { s->top = -1; }
int isEmptyChar(CharStack* s) { return s->top == -1; }

void pushChar(CharStack* s, char c) {
    if (s->top == MAX - 1) {
        printf(RED "❌ Stack Overflow!\n" RESET);
        exit(1);
    }
    s->items[++(s->top)] = c;
}

char popChar(CharStack* s) {
    if (isEmptyChar(s)) {
        printf(RED "❌ Stack Underflow!\n" RESET);
        exit(1);
    }
    return s->items[(s->top)--];
}

char peekChar(CharStack* s) {
    if (isEmptyChar(s)) return '\0';
    return s->items[s->top];
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
    }
    return 0;
}

// ===================== DOUBLE STACK (for evaluation) =====================
typedef struct {
    double items[MAX];
    int top;
} DoubleStack;

void initDouble(DoubleStack* s) { s->top = -1; }
int isEmptyDouble(DoubleStack* s) { return s->top == -1; }

void pushDouble(DoubleStack* s, double val) {
    if (s->top == MAX - 1) {
        printf(RED "❌ Stack Overflow!\n" RESET);
        exit(1);
    }
    s->items[++(s->top)] = val;
}

double popDouble(DoubleStack* s) {
    if (isEmptyDouble(s)) {
        printf(RED "❌ Stack Underflow!\n" RESET);
        exit(1);
    }
    return s->items[(s->top)--];
}

// ===================== QUEUE (History) =====================
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
        q->front = (q->front + 1) % 10;
        q->count--;
    }
    q->rear = (q->rear + 1) % 10;
    strcpy(q->expressions[q->rear], expr);
    q->count++;
}

void showHistory(Queue* q) {
    printf(BOLD YELLOW "\n📜 Expression History:\n" RESET);
    if (q->count == 0) {
        printf("No history available.\n");
        return;
    }
    for (int i = 0; i < q->count; i++) {
        int idx = (q->front + i) % 10;
        printf("%d. %s\n", i+1, q->expressions[idx]);
    }
}

void clearHistory(Queue* q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
    printf(GREEN "✅ History cleared!\n" RESET);
}

// ===================== INFIX → POSTFIX =====================
void infixToPostfix(char* infix, char* postfix) {
    CharStack s;
    initChar(&s);
    int k = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char c = infix[i];

        if (isdigit(c) || c == '.') {
            // multi-digit or float
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[k++] = infix[i++];
            }
            postfix[k++] = ' ';
            i--;
        }
        else if (c == '(') {
            pushChar(&s, c);
        }
        else if (c == ')') {
            while (!isEmptyChar(&s) && peekChar(&s) != '(') {
                postfix[k++] = popChar(&s);
                postfix[k++] = ' ';
            }
            if (isEmptyChar(&s)) {
                printf(RED "❌ Unbalanced parentheses!\n" RESET);
                exit(1);
            }
            popChar(&s);
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            while (!isEmptyChar(&s) &&
                  ((precedence(peekChar(&s)) > precedence(c)) ||
                   (precedence(peekChar(&s)) == precedence(c) && c != '^')) &&
                   peekChar(&s) != '(') {
                postfix[k++] = popChar(&s);
                postfix[k++] = ' ';
            }
            pushChar(&s, c);
        }
        else if (isspace(c)) {
            continue;
        }
        else {
            printf(RED "❌ Invalid character '%c'!\n" RESET, c);
            exit(1);
        }
    }

    while (!isEmptyChar(&s)) {
        if (peekChar(&s) == '(') {
            printf(RED "❌ Unbalanced parentheses!\n" RESET);
            exit(1);
        }
        postfix[k++] = popChar(&s);
        postfix[k++] = ' ';
    }

    postfix[k] = '\0';
}

// ===================== POSTFIX EVALUATION =====================
double evaluatePostfix(char* postfix) {
    DoubleStack s;
    initDouble(&s);

    int i = 0;
    while (postfix[i] != '\0') {
        if (isspace(postfix[i])) {
            i++;
            continue;
        }

        if (isdigit(postfix[i]) || postfix[i] == '.') {
            char numStr[30];
            int k = 0;
            while (isdigit(postfix[i]) || postfix[i] == '.') {
                numStr[k++] = postfix[i++];
            }
            numStr[k] = '\0';
            double num = atof(numStr);
            pushDouble(&s, num);
        }
        else if (postfix[i] == '+' || postfix[i] == '-' ||
                 postfix[i] == '*' || postfix[i] == '/' || postfix[i] == '^') {

            double val2 = popDouble(&s);
            double val1 = popDouble(&s);

            switch (postfix[i]) {
                case '+': pushDouble(&s, val1 + val2); break;
                case '-': pushDouble(&s, val1 - val2); break;
                case '*': pushDouble(&s, val1 * val2); break;
                case '/':
                    if (val2 == 0) {
                        printf(RED "❌ Division by zero!\n" RESET);
                        exit(1);
                    }
                    pushDouble(&s, val1 / val2);
                    break;
                case '^':
                    pushDouble(&s, pow(val1, val2));
                    break;
            }
        }
        i++;
    }

    return popDouble(&s);
}

// ===================== MENU =====================
void showMenu() {
    printf(BOLD CYAN "\n======= EXPRESSION EVALUATOR =======\n" RESET);
    printf(YELLOW "1. Evaluate Expression\n" RESET);
    printf(YELLOW "2. Show History\n" RESET);
    printf(YELLOW "3. Clear History\n" RESET);
    printf(YELLOW "4. Exit\n" RESET);
    printf(BLUE "------------------------------------\n" RESET);
    printf(GREEN "Enter your choice: " RESET);
}

// ===================== MAIN =====================
int main() {
    char infix[100], postfix[100];
    Queue history;
    initQueue(&history);
    int choice;

    do {
        showMenu();
        scanf("%d", &choice);
        getchar(); // clear newline from buffer

        switch (choice) {
            case 1: {
                printf(YELLOW "\nEnter infix expression: " RESET);
                scanf(" %[^\n]", infix);

                infixToPostfix(infix, postfix);
                printf(CYAN "Postfix: %s\n" RESET, postfix);

                double result = evaluatePostfix(postfix);
                printf(GREEN "✅ Result: %.2lf\n" RESET, result);

                char record[120];
                sprintf(record, "%s = %.2lf", infix, result);
                enqueue(&history, record);
                break;
            }
            case 2:
                showHistory(&history);
                break;
            case 3:
                clearHistory(&history);
                break;
            case 4:
                printf(MAGENTA "👋 Exiting program. Goodbye!\n" RESET);
                break;
            default:
                printf(RED "❌ Invalid choice! Try again.\n" RESET);
        }

    } while (choice != 4);

    return 0;
}
