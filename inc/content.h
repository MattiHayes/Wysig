
#ifndef _CONTENT_H_
#define _CONTENT_H_

#define MAX_LINE_LENGTH 100 
#define MAX_LINES 100

typedef struct {
    char content[MAX_LINE_LENGTH];
    int len;
    int dirty;
} Line;


typedef struct {
    Line lines[MAX_LINES];
    int current_line;
    int current_column;
    int num_lines;
} ContentManager;


void add_char_to_line(Line *line, char c);
void create_new_line(ContentManager *cm);
void add_char(ContentManager *cm, char c);
void add_new_line(ContentManager *cm);
void backspace_char(ContentManager *cm);
void save_file(ContentManager *cm, char *file_name);

void curser_up(ContentManager *cm);
void curser_down(ContentManager *cm);
void curser_right(ContentManager *cm);
void curser_left(ContentManager *cm);
void print_curser_location(ContentManager *cm);

#endif // _CONTENT_H_
