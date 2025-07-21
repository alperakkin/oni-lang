#include <stdbool.h>
void raise_error(char *msg, char *symbol);
bool is_oni_file(char *filename);
char *read_file(char *filename);
int is_identifier_char(char c);
int is_identifier_start(char c);