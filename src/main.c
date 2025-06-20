#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "terminal.h"
#include "process_chars.h"

int main(){
    printf("Welcome to Wysig!\n");
    uncook_the_terminal();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c) {
        process_char(c);
    }
    return 0;
}
