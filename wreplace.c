#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"
#include "fileread.h"
#include "wreplace.h"

#define MAX_LINE_LENGTH 1000


char line[MAX_LINE_LENGTH];
char modifiedLine[MAX_LINE_LENGTH];
int linesChanged = 0;
int changedWords = 0;
int linesRead = 0;


char* caseInsensitive(const char *str, const char *target) {
    while (*str) {
        int i = 0;
        while (i < strlen(str) && i < strlen(target) && tolower(str[i]) == tolower(target[i])) {
            i++;
        }

        if (i == strlen(target)){
            return (char *)str;
        }
        str++;
    }
    return NULL;
}

// convert contents to uppercase
void toUpperCase(char *str) {
  for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int processLine(int *applyAll, int *linesChanged, int *changedWords, 
                char *originalLine, char *modifiedLine, FILE *file, 
                int linesRead, int wordCount) {
    if (strcmp(originalLine, modifiedLine) == 0) {
        return 0;
    }
    int confirm = 0;
    if (!(*applyAll) && (linesRead == 1 || wordCount)) {
        confirm = confirmation(originalLine, modifiedLine, linesRead);
    }

    if (confirm == 2) {  // user selects all
        *applyAll = 1;
    }

    if (*applyAll || confirm == 0) {  
        if (wordCount) {
            (*linesChanged)++;
            (*changedWords) += wordCount;
        }
        fseek(file, -strlen(modifiedLine), SEEK_CUR);  // move file pointer to start of line
        fputs(modifiedLine, file);  
    } else if (confirm == -1) {  // skip line
        return -1;
    } else if (confirm == 1) {  // quit
        summary(*changedWords, *linesChanged);
        fclose(file);
        return 1; 
    }
	
    return 0;  
}

int processCommand(const char *command, const char *wordGiven, const char *fileName) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int applyAll = 0;

    fp = fopen(fileName, "r+");
    if (fp == NULL) {
        print_error("Error opening file");
	exit(3);
    }

    int (*commandFunction)(char*, const char*);

    if (strcmp(command, "RC") == 0) {
        commandFunction = &replaceCaseSensitive;
    } else if (strcmp(command, "RI") == 0) {
        commandFunction = &replaceIgnoreCase;
    } else if (strcmp(command, "UK") == 0) {
        commandFunction = &unremoveKeepCase;
    } else if (strcmp(command, "UM") == 0) {
        commandFunction = &unremoveMatchCase;
    } else {
        print_error("Unknown command");
        return 3;
    }

    linesRead = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        linesRead++;
        char originalLine[strlen(line)];  
        strcpy(originalLine, line);  
        int currentChanges = commandFunction(line, wordGiven);
        int result = processLine(&applyAll, &linesChanged, &changedWords, 
                                 originalLine, line, fp, 
                                 linesRead, currentChanges);
        
        if (result == 1) {  // user quit
            fclose(fp);
            free(line);
            return 1;
        }
    }

    fclose(fp);
    if (line) {
        free(line);
    }

    summary(changedWords, linesChanged);
    return 0;
}

int replaceCaseSensitive(char *line, const char *wordGiven) {
    int wordCount = 0;
    char* fileLine = line;
    while (fileLine != NULL) {
        fileLine = strstr(fileLine, wordGiven);
        if (fileLine != NULL) {
            memset(fileLine, '*', strlen(wordGiven)*sizeof(char));
            fileLine = fileLine + strlen(wordGiven); 
            wordCount++;
        }
    }
    return wordCount;
}

int replaceIgnoreCase(char *line, const char *wordGiven) {
    int wordCount = 0;
    char* fileLine = line;
    while (fileLine != NULL) {
        fileLine = caseInsensitive(fileLine, wordGiven);
        if (fileLine != NULL) {
            memset(fileLine, '*', strlen(wordGiven)*sizeof(char));
            fileLine = fileLine + strlen(wordGiven); 
            wordCount++;
        }
    }
    return wordCount;
}

int unremoveKeepCase(char *line, const char *wordGiven) {
    char stars[strlen(wordGiven)];
    strcpy(stars, "");
    for (int i = 0; i < strlen(wordGiven); i++) {
        strcat(stars, "*");
    }
    int wordCount = 0;
    char* fileLine = line;
    while (fileLine != NULL) {
        fileLine = strstr(fileLine, stars);
        if (fileLine != NULL) {
            for (int i = 0; i < strlen(wordGiven); i++) {
                memset(fileLine, wordGiven[i], sizeof(char));    
                fileLine++; 
            }
            wordCount++;
        }
    }
    return wordCount;
}

int unremoveMatchCase(char *line, const char *wordGiven) {
    char stars[strlen(wordGiven)];
    strcpy(stars, "");
    for (int i = 0; i < strlen(wordGiven); i++) {
        strcat(stars, "*");
    }
    int wordCount = 0;
    char* fileLine = line;
    while (fileLine != NULL) {
        fileLine = strstr(fileLine, stars);
        if (fileLine != NULL) {
            char* prev = fileLine >= line ? fileLine - 1: fileLine;
            char* after = fileLine + strlen(wordGiven);
            int (*convertCase)(int ch);
            if (isalpha(prev[0])) { // true if no space
                if (islower(prev[0])) {
                    convertCase = &tolower;
                } else {
                    convertCase = &toupper;
                }
            } else if (isalpha(after[0])) {
                if (islower(after[0])) {
                    convertCase = &tolower;
                } else {
                    convertCase = &toupper;
                }
            } else {
                convertCase = NULL;
            }
            for (int i = 0; i < strlen(wordGiven); i++) {
                int ch;
                if (convertCase == NULL) {
                    ch = wordGiven[i];
                } else {
                    ch = convertCase(wordGiven[i]);
                }
                memset(fileLine, ch, sizeof(char));
                fileLine++; 
            }
            wordCount++;
        }
    }
    return wordCount;
}