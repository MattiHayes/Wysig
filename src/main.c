#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "terminal.h"

void process_char(char c){
    switch (c){
        case 17:
            exit(0);
            break;
        case '\r':
            write(STDOUT_FILENO, "\r\n", 2);
            break;
        case 127:
            write(STDOUT_FILENO, "\b \b", 3);
            break;
        default:
            write(STDOUT_FILENO, &c, 1);
    }
}


int main(){
    printf("Welcome to Wysig!\n");
    uncook_the_terminal();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c) {
        process_char(c);
    }
    return 0;
}
