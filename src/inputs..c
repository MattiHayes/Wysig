#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "inputs.h"
#include "screen.h"
#include "content.h"

#define NEWLINE_RETURN "\r\n"
#define NEWLINE_RETURN_LEN 2

enum process_commands {
    ENTER = 'm' - 'a' + 1,
    QUIT = 'q' - 'a' + 1,
    SAVE = 's' - 'a' + 1
};

enum arrow_keys{
    UP = 'A',
    DOWN = 'B',
    RIGHT = 'C',
    LEFT = 'D'
};


ContentManager cm = {0};


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
        // sorry escape codes are not supported at the moment
        // if (code[0] == '[') { 
        //     process_escape_code(code[1]);
        // }
    } else {
        switch (c){
            case 127:
                write(STDOUT_FILENO, "\b \b", 3);
                backspace_char(&cm);
                break;
            default:
                write(STDOUT_FILENO, &c, 1);
                add_char(&cm, c);
        }
    }
}

void process_ctrl(char c){
    switch (c){
        case ENTER:
            write(STDOUT_FILENO, NEWLINE_RETURN, NEWLINE_RETURN_LEN);
            add_new_line(&cm);
            break;
        case SAVE: 
            write(STDOUT_FILENO, "\r\n\n...no backspace in save yet...\r\n", 35);
            write(STDOUT_FILENO, "\r\nEnter file name and enter: ", 29);
            char name[30];
            char name_c;
            for (int i = 0; read(STDIN_FILENO, &name_c, 1) == 1 && name_c != '\r'; i++){
                write(STDOUT_FILENO, &name_c, 1);
                name[i] = name_c;
            }
            save_file(&cm, name);
            write(STDOUT_FILENO, NEWLINE_RETURN, NEWLINE_RETURN_LEN);
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
            curser_up(&cm);
            break;
        case DOWN:
            manipulate_terminal(STDOUT_FILENO, &CURSOR_DOWN);
            curser_down(&cm);
            break;
        case RIGHT:
            manipulate_terminal(STDOUT_FILENO, &CURSOR_RIGHT);
            curser_right(&cm);
            break;
        case LEFT:
            manipulate_terminal(STDOUT_FILENO, &CURSOR_LEFT);
            curser_left(&cm);
            break;
        
    }
}
