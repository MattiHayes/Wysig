#include <terminal.h>

struct termios original_state;

void cook_the_terminal(){
    tcsetattr(STDIN_FILENO, TCSANOW, &original_state);
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

int uncook_the_terminal(){
    atexit(cook_the_terminal);
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
