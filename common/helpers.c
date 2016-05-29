#include "helpers.h"

#include <unistd.h>
#include <string.h>

void write_to_file(int fd, char *message)
{
    write(fd, message, strlen(message) + 1);
}

void print_error(char *error)
{
    write_to_file(STDERR_FILENO, error);
}

void print_message(char *message)
{
    write_to_file(STDOUT_FILENO, message);
}
