#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#include "inputs.h"
#include "terminal.h"
#include "screen.h"


int main(){
    manipulate_terminal(STDOUT_FILENO, &CLEAR_SCREEN);
    
    // I'll add these back in later when I focus on the display
    // printf("--------------------------------------\n");
    // printf("Wysig | Quit:CTRL+Q\n");
    printf("\n");
    manipulate_terminal(STDOUT_FILENO, &CURSER_HOME);
    uncook_the_terminal();
    

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c) {
        process_char(c);
    }
    return 0;
}
