# Wysig

**Wysig** is a terminal based text editor written in C. 

This project was inspired by [kilo](https://viewsourcecode.org/snaptoken/kilo/index.html). I have 
looked at parts of the kilo tutorial however, my goal is to build this as independently as possible. 
I will write my own code  mostly from scratch. Google is my first stop when I get stuck and I use AI
 as a sounding board - not as a code generator. 

## TODO:

- [ ] Read up on termios.h
- [ ] **Screen**
    - [ ] update the screen
    - [ ] Clean and redraw the screen
- [ ] **Inputs**
    - [ ] Filter inputs
    - [ ] Display typed charters
    - [ ] Store inputs
- [ ] (When done) update README to past tense 🥳


## Explanation of Parts
### 🥩 'Raw' Mode 
## Explanation of Parts

### 🥩 'Raw' Mode

So the first step was to disable "cooked" mode — where the terminal echoes what has been entered from the keyboard. This required the use of `termios.h`.

The `termios` struct essentially controls the settings of the terminal. The settings are split into different sections: _local modes_, _input modes_, _output modes_, _control modes_, and _control characters_ (special chars).

For example, setting the `ECHO` flag in the local flags will echo inputs to the terminal.

If you disable most of the echo-related and input-processing flags, and then use `write(...)` to print input characters, nothing happens — at least not immediately. That’s because the control mode also needs to be set to expect eight bits (`CS8`), and the control character array (`c_cc`) must have `VMIN` set to 1 (minimum of one byte to return the input) and `VTIME` set to 0 (no delay before returning input — this is in tenths of a second, by the way). Without these, the terminal doesn't deliver characters as you type.

Now with the correct configuration, characters are printed to the screen. The only problem is that pressing Enter just causes new characters to overwrite the current line instead of going to a new line.

So I tried the code below, which should print the newline and also count the number of times there is a newline:

```c
...
int lines = 1; 
if (c == '\n') {
    write(STDOUT_FILENO, &c, 1);
    lines++;
}
```
But then lines was always  zero ... 😭.
```
I needed a way to tell whether a character was a control character or a printable one. After some 
playing around (and frustration), I peeked at the kilo tutorial and discovered `iscntrl()`, which 
does exactly that.

And that, dear reader, brings us to the next part of the adventure.

### 👻 Non-printable Characters 