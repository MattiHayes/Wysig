#include "process_chars.h"

#define NEWLINE_RETURN "\r\n"
#define NEWLINE_RETURN_LEN 2

enum process_commands {
    BACKSPACE_DEL = 'm' - 'a' + 1,
    QUIT = 'q' - 'a' + 1,
    SAVE = 's' - 'a' + 1
}; 

void process_char(char c){
    if (0 < c && c < 27) {
        process_ctrl(c);
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
        case QUIT:
            exit(0);
            // break; (good practice)
    }
}
