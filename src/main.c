#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios original_state;

void clean_exit(){
    printf("\n");
    printf("Cleaning up your mess.\n");
    tcsetattr(STDIN_FILENO, TCSANOW, &original_state);

}

int disable_echoing(struct termios t){
    tcgetattr(STDIN_FILENO, &original_state);
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    return 0;
}


int main(){
    atexit(clean_exit);
    printf("Welcome to Wysig!\n");

    // TODO: figure out how so shave the character to a buffer
    // TODO: figure out how to write the buffer to a file!
    struct termios t;
    
    if (disable_echoing(t)) {
        printf("Could not enter raw mode.");
        clean_exit(t); // although the disabeling of echoing should not have been completed ... 
    }

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        switch (c) {
            case '\n':
                printf("New line detected!\n");
                break;
            case 'q':
                break;
        }
    }

    return 0;
}
