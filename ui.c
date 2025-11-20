#include <stdio.h>
#include <string.h>  


void print_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

int confirmation(const char *before, const char *after, int lineno) {
    char input[5];  // buffer for user input
    
    printf("\nChanging line %d from\n%sto\n%s", lineno, before, after);
    printf("Confirm (Yes, No, All, Quit)? ");
    
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // remove newline character

    // handle input cases
    if (strcasecmp(input, "Yes") == 0 || strcasecmp(input, "Y") == 0) {
        return 0; // accepted
    } else if (strcasecmp(input, "No") == 0 || strcasecmp(input, "N") == 0) {
        return -1; // skipped
    } else if (strcasecmp(input, "All") == 0 || strcasecmp(input, "A") == 0) {
        return 2;  // all 
    } else if (strcasecmp(input, "Quit") == 0 || strcasecmp(input, "Q") == 0) {
        return 1; // quit
    } else {
        print_error("Invalid input");
    }
    return 3;  
}

int summary(int changes, int linesChanged) {
    if (changes == 0) {
        printf("No changes were made.\n");
        return 2; 
    } else {
        printf("Changed %d word(s) in %d line(s).\n", changes, linesChanged);
        return 0; 
    }
}
