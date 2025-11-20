#ifndef WREPLACE_H
#define WREPLACE_H

char* caseInsensitive(const char *str, const char *target);
void toUpperCase(char *contents);
int processCommand(const char *command, const char *wordGiven, const char *fileName);
int processLine(int *applyAll, int *linesChanged, int *changedWords, 
                char *originalLine, char *modifiedLine, FILE *tempFile, 
                int linesRead, int wordCount);
int replaceCaseSensitive(char *line, const char *wordGiven);
int replaceIgnoreCase(char *line, const char *wordGiven);
int unremoveKeepCase(char *line, const char *wordGiven);
int unremoveMatchCase(char *line, const char *wordGiven);

#endif 