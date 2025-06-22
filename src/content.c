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
    } else {
        printf("You ran out of lines and should probably think of some way to fix this.");
    }
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

void save_file(ContentManager *cm, char *file_name){
    FILE * output;
    add_new_line(cm);
    output = fopen(file_name, "w");
    // this only saves up to the last new line char
    for (int i = 0; i < cm->current_line + 1; i++) {
        fputs(cm->lines[i].content, output);
    }
    fclose(output);
}
