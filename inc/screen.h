/**
 * @file screen_commands.h
 * @author Matthew Hayes
 * @brief Functions to manipulate the appearance of the terminal.
 * @version 0.1
 * @date 2025-06-21
 * 
 */

#ifndef _SCREEN_H_
#define _SCREEN_H_

typedef struct {
    const char * sequence;
    const int len;
} TerminalCommand;

extern const TerminalCommand CLEAR_SCREEN;

extern const TerminalCommand CURSOR_UP;
extern const TerminalCommand CURSOR_DOWN;
extern const TerminalCommand CURSOR_RIGHT;
extern const TerminalCommand CURSOR_LEFT;
extern const TerminalCommand CURSER_HOME;


void manipulate_terminal(int fd, const TerminalCommand *command);

#endif // _SCREEN_H_
