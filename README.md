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
    - [x] Filter inputs
    - [ ] Display typed charters
    - [ ] Store inputs
- [ ] (When done) update README to past tense 🥳



## Explanation of Parts

### 🥩 'Raw' Mode

So the first step was to disable "cooked" mode — where the terminal echoes what has been entered 
from the keyboard. This required the use of `termios.h`.

The `termios` struct essentially controls the settings of the terminal. The settings are split into 
different sections: _local modes_, _input modes_, _output modes_, _control modes_, and _control 
characters_ (special chars).

For example, setting the `ECHO` flag in the local flags will echo inputs to the terminal.

If you disable most of the echo-related and input-processing flags, and then use `write(...)` to 
print input characters, nothing happens — at least not immediately. That’s because the control mode 
also needs to be set to expect eight bits (`CS8`), and the control character array (`c_cc`) must have
 `VMIN` set to 1 (minimum of one byte to return the input) and `VTIME` set to 0 (no delay before 
 returning input — this is in tenths of a second, by the way). Without these, the terminal doesn't 
 deliver characters as you type.

Now with the correct configuration, characters are printed to the screen. The only problem is that 
pressing Enter just causes new characters to overwrite the current line instead of going to a new 
line.

So I tried the code below, which should print the newline and also count the number of new line 
characters:

```c
...
int lines = 0; 
if (c == '\n') {
    write(STDOUT_FILENO, &c, 1);
    lines++;
...
}
```
But then lines was always  zero ... 😭.

I needed a way to tell whether a character was a control character or a printable one. After some 
playing around (and frustration), I peeked at the kilo tutorial and discovered `iscntrl()`, which 
does exactly that.

And that, dear reader, brings us to the next part of the adventure.

### 👻 Non-printable Characters 
Turns out one of those flags I mentioned earlier disables the terminal's automatic mapping of the 
carriage return character (`\r`) to the newline character (`\n`). Yes that means that when we
press enter the computer reads a `\r` not a `\n` (at least on linux) ....  **AAAAAHHHHHHHH!!!!**

OK. Now that I'm back (sorry for that outburst, by the way), let's have a quick, calm discussion 
about carriage return and newline characters.

According to [this Reddit post](https://www.reddit.com/r/learnprogramming/comments/k76du/why_both_r_and_n_to_go_to_the_next_line/)
, this all goes back to how commands were sent to real 
printers. The carriage return (`\r`) moves the cursor to the beginning of the line, and the newline 
(`\n`) moves the cursor down to the next line.

But — and here's the kicker — different operating systems chose to represent the Enter key using 
different combinations of these characters.

**WHYYYYYY**  (sorry ... again)

*Deep breaths.*

OK, so this leads me back to the tutorial. Using this snippet of code, we can see what each keyboard
input is actually read as by the computer:

```c
char c;
while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
        printf("%d\r\n", c);
    } else {
        printf("%c = %d\r\n", c, c);
    }
}
```

When I was 13 this was exactly how I thought I would be spending my Friday nights as an adult ... 
And you know what? I'm not even mad about it.

OK, back to how your — well, my — keyboard reads inputs.

ASCII characters 1 through 26 correspond to your `Ctrl + <char>` inputs, where the ASCII number is 
just the letter’s position in the alphabet. So `Ctrl+A` is 1, `Ctrl+B` is 2, and so on.

Also, our favorite character — carriage return — is **13**
and the Backspace key is mapped to **ASCII 127**, not `'\b'` or ASCII 8 like you might expect. 
(**More deep breaths**). Then we get to these guys called *escape sequences*. Oh — before we move on
 — if you were wondering, the ✨Null✨ character (Rust, I’m looking at you) is `Ctrl+@`.  
That’s `Ctrl+Shift+<whatever the @ is on your keyboard>`. 

I'm now thinking of renaming this project to **WHYsig**

Ok back to the code. Now that we've got a handle on raw input (at least we think we do), we can 
modify the code to actually get the `'q'` character back when it’s typed. It was also around this 
point that I decided it was time to start splitting my code into different source files.