/**
 * @file process_chars.h
 * @author Matthew Hayes
 * @brief Functions to process the inputs from the  keyboard
 * @version 0.1
 * @date 2025-06-20
 * 
 */

#ifndef _PROCESS_CHARS_H_
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

void process_char(char c);
void process_ctrl(char c);

#endif // _PROCESS_CHARS_H_
