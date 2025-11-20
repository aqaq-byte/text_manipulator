#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r+");
    if (file == NULL) {
        print_error("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    if (size == -1) {
        print_error("Error getting file size");
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    // if file is empty, return empty string
    if (size == 0) {
        fclose(file);
        return calloc(1, 1);  // one byte for \0
    }

    // allocate memory for file contents
    char *contents = malloc(size + 1);
    if (contents == NULL) {
        print_error("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    fclose(file);
    return contents;
}
