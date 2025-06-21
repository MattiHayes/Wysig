#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "process_chars.h"
#include "screen.h"

#define NEWLINE_RETURN "\r\n"
#define NEWLINE_RETURN_LEN 2

enum process_commands {
    BACKSPACE_DEL = 'm' - 'a' + 1,
    QUIT = 'q' - 'a' + 1,
    SAVE = 's' - 'a' + 1
};

enum arrow_keys{
    UP = 'A',
    DOWN = 'B',
    RIGHT = 'C',
    LEFT = 'D'
};

void process_char(char c){
    if (0 < c && c < 27) {
        process_ctrl(c);
    } else if (c == 27) {
        // escale code
        char code[3];
        for (int i = 0; i < 2; i++){
            if (read(STDIN_FILENO, &code[i], 1) != 1) {
                return;
            } 
        }
        if (code[0] == '[') { 
            process_escape_code(code[1]);
        }
    } else {
        switch (c){
            case 127:
                write(STDOUT_FILENO, "\b \b", 3);
                break;
            default:
                write(STDOUT_FILENO, &c, 1);
        }
    }
}

void process_ctrl(char c){
    switch (c){
        case BACKSPACE_DEL:
            write(STDOUT_FILENO, NEWLINE_RETURN, NEWLINE_RETURN_LEN);
            break;
        case SAVE:
            write(STDOUT_FILENO, NEWLINE_RETURN, NEWLINE_RETURN_LEN);
            write(STDOUT_FILENO, "Save to file: ", 15);
            break;
        case QUIT:
            exit(0);
            // break; (good practice)
    }
}

void process_escape_code(char code){
    // lets start off with the arrow keys.
    // ESC [ A for up, ESC [ B for down, ESC [ C for right, and ESC [ D for left. 
    switch (code) {
        case '\0':
        case 27:
            // lets just get these ones out of the way
            break;
        case UP:
            manipulate_terminal(STDOUT_FILENO, &CURSOR_UP);
            break;
        case DOWN:
            manipulate_terminal(STDOUT_FILENO, &CURSOR_DOWN);
            break;
        case RIGHT:
            manipulate_terminal(STDOUT_FILENO, &CURSOR_RIGHT);
            break;
        case LEFT:
            manipulate_terminal(STDOUT_FILENO, &CURSOR_LEFT);
            break;
        
    }
}
