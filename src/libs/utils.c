#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

bool is_oni_file(char *filename)
{

    size_t length = strlen(filename);
    if (length < 4)
    {
        return false;
    }
    const char *ext = filename + (length - 4);
    return strcmp(ext, ".oni") == 0;
}
void raise_error(char *msg, char *symbol)
{
    fprintf(stderr, "%s -> %s\n", msg, symbol);
    exit(1);
}

char *read_file(char *filename)
{
    if (!is_oni_file(filename))
        raise_error("Please use -.oni- as file extension!", filename);
    FILE *file = fopen(filename, "rb");

    if (!file)
    {
        raise_error("File not found!", filename);
    }

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    if (filesize < 0)
    {
        fclose(file);
        raise_error("Could not get file size", filename);
    }
    char *buffer = malloc(filesize + 1);
    if (!buffer)
    {
        fclose(file);
        raise_error("Memory could not be allocated", filename);
    }

    size_t read_size = fread(buffer, 1, filesize, file);
    if (read_size != filesize)
    {
        fclose(file);
        raise_error("File could not be read", filename);
    }

    buffer[filesize] = '\0';
    fclose(file);
    return buffer;
}