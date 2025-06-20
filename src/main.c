#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "terminal.h"


int main(){
    printf("Welcome to Wysig!\n");
    uncook_the_terminal();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)){
            printf("%d \r\n", c);
        } else {
            printf("%c = %d \r\n", c, c);
            //write(STDOUT_FILENO, &c, 1);
        }
    }
    return 0;
}
