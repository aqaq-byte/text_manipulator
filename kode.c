#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileread.h"
#include "wreplace.h"
#include "ui.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: kode <command> <word> <file>\n");
        return 3;
    }

    const char *command = argv[1];
    const char *word = argv[2];
    const char *fileName = argv[3];
     
    return processCommand(command, word, fileName);
}
