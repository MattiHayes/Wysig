#include "content.h"
#include "screen.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>



void move_curser_to_end_of_next_line(ContentManager *cm);

// -------------------------------------------------------------------------------------------------
// Main functions
// -------------------------------------------------------------------------------------------------


// TODO: I think I can clean this code up a bit
void add_char(ContentManager *cm, char c){
    Line line = cm->lines[cm->current_line];

    if (line.len < MAX_LINE_LENGTH - 1) {
        // there is two spaces left in the string
        add_char_to_line(&cm->lines[cm->current_line], c);
        cm->current_column++;
    } else if (cm->lines[cm->current_line].len == MAX_LINE_LENGTH - 1){
        // we are at capacity in the current string
        add_char_to_line(&cm->lines[cm->current_line], '\0');
        create_new_line(cm);
        add_char_to_line(&cm->lines[cm->current_line], c);
        cm->current_column++;
    } else {
        printf("Ok I don't actually know what to do here");
    }
}

void backspace_char(ContentManager *cm){
    if (cm->current_column > 0) {
        cm->current_column--;
        cm->lines[cm->current_line].len--;
    } else {
        move_curser_to_end_of_next_line(cm);
        // there also  needs to be a merge of lines now ...
    }
}

// -------------------------------------------------------------------------------------------------
// helper functions
// -------------------------------------------------------------------------------------------------

void add_char_to_line(Line *line, char c) {
    line->content[line->len] = c;
    line->len++;
}


void add_new_line(ContentManager *cm){
    add_char(cm, '\n');
    add_char(cm, '\0');
    create_new_line(cm);
}

void create_new_line(ContentManager *cm){
    if (cm->current_line < MAX_LINES) {
        cm->current_line++;
        cm->num_lines++;
        cm->current_column = 0;
    } else {
        printf("You ran out of lines and should probably think of some way to fix this.");
    }
}

// -------------------------------------------------------------------------------------------------
// Moving curser funcations  --- maybe should be in their own file as well 
// -------------------------------------------------------------------------------------------------

void move_curser_to_end_of_next_line(ContentManager *cm){
    if (cm->current_line == 0) {
        return;
    }
    cm->current_line--;
    int len = cm->lines[cm->current_line].len;
    cm->current_column = len;
}

void curser_up(ContentManager *cm){
    //int line_len = cm->lines[cm->current_line].len;
    if (cm->current_line > 0) cm->current_line -= 1;
}

void curser_down(ContentManager *cm){
    if (cm->current_line >= cm->num_lines) {
        add_new_line(cm);
        return;
    }
    if (cm->current_line == MAX_LINES) {
        return;
    }
    
    cm->current_line++;
    if (cm->current_column > cm->lines[cm->current_line].len) {
        cm->current_column = cm->lines[cm->current_line].len;
    }
}

void curser_right(ContentManager *cm){
    Line *line = &cm->lines[cm->current_line];
    if (line->len+1 > MAX_LINE_LENGTH) {
        //add_new_line(cm);
        return; 
    }
    if (cm->current_column == line->len) {
        add_char_to_line(line, ' ');
    }
    cm->current_column++;
    
}

void curser_left(ContentManager *cm){
    if (cm->current_column == 0) {
        move_curser_to_end_of_next_line(cm);
        return;
    }
    if (cm->current_column > 0) cm->current_column--;
}


// -------------------------------------------------------------------------------------------------
// Misc probably will move these into a separate file later
// -------------------------------------------------------------------------------------------------
void print_curser_location(ContentManager *cm) {
    write(STDOUT_FILENO, "\033[H", 3);
    char pos[44];
    sprintf(
        pos,
        "%2d:%2d | Line length: %2d Number of lines %2d",
        cm->current_column, cm->current_line,
        cm->lines[cm->current_line].len,
        cm->num_lines + 1
    );
    write(STDOUT_FILENO, &pos, strlen(pos));
    sprintf(pos, "\033[%d;%dH", cm->current_line + 2, cm->current_column+1);
    write(STDOUT_FILENO, &pos, strlen(pos));
}


void save_file(ContentManager *cm, char *file_name){
    FILE *output;
    add_new_line(cm);
    output = fopen(file_name, "w");
    //this only saves up to the last new line char
    for (int i = 0; i <= cm->num_lines; i++) {
        fputs(cm->lines[i].content, output);
    }
    fclose(output);
}
