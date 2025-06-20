/**
 * @file terminal.h
 * @author Matthew Hayes
 * @brief Function to configure the terminal for raw mode.
 * @version 0.1
 * @date 2025-06-20
 * 
 */

#ifndef _TERMINAL_H_
#define _TERMINAL_H_
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void configure_local_modes(struct termios *t);
void configure_input_modes(struct termios *t);
void configure_output_modes(struct termios *t);
void configure_control_modes(struct termios *t);
void configure_control_characters(struct termios *t);

/**
 * @brief Set the terminal to non-canonical mode.
 * 
 * @return int 0 or -1 for error.
 */
int uncook_the_terminal(); 

/**
 * @brief Set the terminal to its state before we set it to non-canonical mode.
 * 
 */
void cook_the_terminal();

#endif // _TERMINAL_H_
