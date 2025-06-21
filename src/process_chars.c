#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "process_chars.h"
#include "screen.h"

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

// -------------------------------------------------------------------------------------------------
typedef struct {
    char *content;
    int len;
    // int dirty; <-- this will eventually be for smart writing/saving 
} Line;


Line lines[100];
int current_line;
char buff[100];
int buff_pos;


void add_char_to_line(char c){
        buff[buff_pos] = c;
        buff_pos++;
}

void add_new_line(){
    buff[buff_pos++] = '\n';
    buff[buff_pos] = '\0';
    lines[current_line].content = strdup(buff);
    lines[current_line].len = buff_pos;
    current_line++;
    buff_pos = 0;
}

void backspace_char(){
    if (buff_pos) --buff_pos;
}

void save_file(){
    add_new_line();
    FILE * output;
    output = fopen("Output.txt", "w");
    // this only saves up to the last new line char
    for (int i = 0; i < current_line; i++) {
        fputs(lines[i].content, output);
    }
}

// -------------------------------------------------------------------------------------------------
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
                backspace_char();
                break;
            default:
                write(STDOUT_FILENO, &c, 1);
                add_char_to_line(c);
        }
    }
}

void process_ctrl(char c){
    switch (c){
        case ENTER:
            write(STDOUT_FILENO, NEWLINE_RETURN, NEWLINE_RETURN_LEN);
            add_new_line();
            break;
        case SAVE:
            //write(STDOUT_FILENO, NEWLINE_RETURN, NEWLINE_RETURN_LEN);
            //write(STDOUT_FILENO, "Save to file: ", 15);
            save_file();
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
