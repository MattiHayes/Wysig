#include "screen.h"
#include <unistd.h>

// screen related commands
const TerminalCommand CLEAR_SCREEN = {"\033[2J", 4};

// curser related commands
const TerminalCommand CURSOR_UP = {"\033[A", 3};
const TerminalCommand CURSOR_DOWN = {"\033[B", 3};
const TerminalCommand CURSOR_RIGHT = {"\033[C", 3};
const TerminalCommand CURSOR_LEFT = {"\033[D", 3};
const TerminalCommand CURSER_HOME = {"\033[H", 3};


void manipulate_terminal(int fd, const TerminalCommand *command){
    write(fd, command->sequence, command->len);
}
