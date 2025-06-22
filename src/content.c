#include "content.h"
#include "stdio.h"


void add_char_to_line(Line *line, char c) {
    line->content[line->len] = c;
    line->len++;
}

void create_new_line(ContentManager *cm){
    if (cm->current_line < MAX_LINES) {
        cm->current_line++;
        cm->lines[cm->current_line].content[0] = '\0';
        cm->num_lines++;
    } else {
        printf("You ran out of lines and should probably think of some way to fix this.");
    }
}


void save_file(ContentManager *cm, char *file_name){
    FILE * output;
    add_new_line(cm);
    output = fopen(file_name, "w");
    // this only saves up to the last new line char
    for (int i = 0; i <= cm->num_lines; i++) {
        fputs(cm->lines[i].content, output);
    }
    fclose(output);
}


void add_char(ContentManager *cm, char c){
    Line line = cm->lines[cm->current_line];

    if (line.len < MAX_LINE_LENGTH - 1) {
        // there is two spaces left in the string
        add_char_to_line(&cm->lines[cm->current_line], c);
    } else if (cm->lines[cm->current_line].len == MAX_LINE_LENGTH - 1){
        // we are at capacity in the current string
        add_char_to_line(&cm->lines[cm->current_line], '\0');
        create_new_line(cm);
        add_char_to_line(&cm->lines[cm->current_line], c);
    } else {
        printf("Ok I don't actually know what to do here");
    }
}

void add_new_line(ContentManager *cm){
    add_char(cm, '\n');
    add_char(cm, '\0');
    create_new_line(cm);
}

void backspace_char(ContentManager *cm){
    cm->lines[cm->current_line].len--;
}


void curser_up(ContentManager *cm){
    //int line_len = cm->lines[cm->current_line].len;
    if (cm->current_line > 0) {
        cm->current_line -= 1;
    }
}

void curser_down(ContentManager *cm){
    if (cm->current_line < MAX_LINES) cm->current_line++;
}

void curser_right(ContentManager *cm){
    Line line = cm->lines[cm->current_line];
    if (line.len+1 > MAX_LINE_LENGTH) {
        //add_new_line(cm); this is commented out so that the behaviour in the file is the same as in the terminal
        return; 
    } else if (line.content[line.len+1] == '\0') {
        line.content[++line.len] = ' '; 
    } else {
        line.len++;
    }
}

void curser_left(ContentManager *cm){
    Line *line = &cm->lines[cm->current_line];
    if (line->len-1 == 0 && cm->current_line > 0) {
        // cm->current_line--; this is commented out so that the behaviour is the same in the file as on the screen
        return;
    } else {
        line->len--;
    }
}
