
#ifndef _CONTENT_H_
#define _CONTENT_H_

#define MAX_LINE_LENGTH 100 
#define MAX_LINES 100

typedef struct {
    char content[MAX_LINE_LENGTH];
    int len;
    // int dirty; <-- this will eventually be for smart writing/saving 
} Line;


typedef struct {
    Line lines[MAX_LINES];
    int current_line;
} ContentManager;


//void add_char_to_line(Line *line, char c);
//void create_new_line(ContentManager *cm);
void add_char(ContentManager *cm, char c);
void add_new_line(ContentManager *cm);
void backspace_char(ContentManager *cm);
void save_file(ContentManager *cm, char *file_name);


#endif // _CONTENT_H_
