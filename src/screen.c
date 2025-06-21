#include "screen.h"
#include <unistd.h>

// screen related commands


// curser related commands
const TerminalCommand CURSOR_UP = {"\033[1A", 4};
const TerminalCommand CURSOR_DOWN = {"\033[1B", 4};
const TerminalCommand CURSOR_RIGHT = {"\033[1C", 4};
const TerminalCommand CURSOR_LEFT = {"\033[1D", 4};


void manipulate_terminal(int fd, const TerminalCommand *command){
    write(fd, command->sequence, command->len);
}
