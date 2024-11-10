#include <stdio.h>
#include <fcntl.h>
#include "get_next_line.h"

int main(){
    int fd = open("get_next_line_utils.c", O_RDONLY);

    char *PTR = NULL;
    while ((PTR = get_next_line(fd)))
    {
        printf("%s", PTR);
        free(PTR);
    }
    close(fd);
    return (0);
}