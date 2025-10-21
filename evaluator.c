#include <stdio.h>
#include <string.h>

// main function
int main() {
    char expression[100]; // to store user input expression

    printf("=====================================\n");
    printf("     EXPRESSION EVALUATOR PROJECT\n");
    printf("=====================================\n\n");

    // ask for user input
    printf("Enter an infix expression (e.g., 3+4*2): ");
    scanf("%s", expression);

    // show the entered expression
    printf("\nYou entered: %s\n", expression);
    printf("[Next Step -> We'll Convert this to Postfix in Day 2]\n");

    return 0;
}
