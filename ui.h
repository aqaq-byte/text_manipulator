#ifndef UI_H
#define UI_H

int confirmation(const char *before, const char *after, int lineno);
void print_error(const char *message);
void summary(int changes, int linesChanged);

#endif
