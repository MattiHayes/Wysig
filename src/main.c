#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

struct termios original_state;


void clean_exit(){
    tcsetattr(STDIN_FILENO, TCSANOW, &original_state);
    printf("\n");
    printf("Cleaning up your mess.\n");
}


void configure_local_modes(struct termios *t){
   //t->c_lflag &= ~(ECHO | ECHOE | ECHOK | ICANON | IEXTEN | ECHONL);  
   t->c_lflag &= ~(ECHO | ICANON);
}


void configure_input_modes(struct termios *t){
    t->c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
}


void configure_output_modes(struct termios *t){
    t->c_oflag &= ~(OPOST);
}


void configure_control_modes(struct termios *t){
        t->c_cflag |= CS8;
}


void configure_control_characters(struct termios *t){
    t->c_cc[VMIN] = 1;
    t->c_cc[VTIME] = 0;
}

int configure_terminal(){
    tcgetattr(STDIN_FILENO, &original_state);  // save the current state
    struct termios raw;
    configure_local_modes(&raw);
    configure_input_modes(&raw);
    // configure_output_modes(&raw);
    // configure_control_modes(&raw);
    configure_control_characters(&raw);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH , &raw) < 0){
        return -1;
    };
    return 0;
}

int main(){
    atexit(clean_exit);
    printf("Welcome to Wysig!\n");

    // TODO: figure out how so shave the character to a buffer
    // TODO: figure out how to write the buffer to a file!
    configure_terminal();

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
