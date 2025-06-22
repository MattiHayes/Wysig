# Wysig

**Wysig** is a terminal based text editor written in C. 

This project was inspired by [kilo](https://viewsourcecode.org/snaptoken/kilo/index.html). I have 
looked at parts of the kilo tutorial however, my goal is to build this as independently as possible. 
I will write my own code  mostly from scratch. Google is my first stop when I get stuck and I use AI
 as a sounding board - not as a code generator. 

## TODO:

- [ ] Read up on termios.h
- [ ] **Screen**
    - [x] update the screen with text
    - [ ] Clean and redraw the screen
- [ ] **Inputs**
    - [x] Filter inputs
    - [x] Display typed charters
    - [ ] **Commands to Implement**
        - [x] Backspace
        - [ ] Tab
        - [ ] Select
        - [ ] Arrow keys when they get to the end/front of a line etc
    - [x] Store inputs
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
(**More deep breaths**). Then we get to these guys called [escape codes](https://en.wikipedia.org/wiki/ANSI_escape_code) ... 😶

 Oh — before we move on
 — if you were wondering, the ✨Null✨ character (Rust, I’m looking at you) is `Ctrl+@`.  
That’s `Ctrl+Shift+<whatever the @ is on your keyboard>`. 

I'm now thinking of renaming this project to **WHYsig**

Ok back to the code. Now that we've got a handle on raw input (at least we think we do), we can 
modify the code to actually get the `'q'` character back when it’s typed. It was also around this 
point that I decided it was time to start splitting my code into different source files.

### 🏃‍♂️ Escape Codes and Terminal Commands

Ok, so now things are kinda coming together. These two topics are grouped together because — 
naturally — they work together. To process an arrow key, the escape code needs to be broken down, 
and then the cursor needs to be moved on the screen... thus: ✨**terminal commands**✨.

Escape codes (or escape sequences — whichever you prefer) are made up of three characters. They 
start with the escape character (`\27`), followed by `[` and then a letter which identifies the 
command. There might be more complex forms out there, but for now, I don't really care about them.

Let’s walk through what happens when the up arrow key is pressed. The characters sent are `\27[A`, 
but the *first* character read is just `\27`. So, I need to read two more characters and make sure 
the next is `[` before processing the third one as the command.

Cool — now we can differentiate the escape codes we need to move the cursor.

So this is kinda cool (just me?): if we write `\033[A` to the terminal, the cursor moves 
**up one row**. Infact, if we write `\033[1A`, that also moves it up one row. So really, `\033[<n>A`
moves the cursor *n* rows up. Ok .. that wasn’t too bad.

Now, I foresee myself writing a lot of these terminal command sequences, so I decided to create a 
`TerminalCommand` struct to store both the command string and its length. Hopefully, this will 
reduce some repetition and improve code clarity. From this idea, these bits of code were born:


```C
const TerminalCommand CURSOR_UP = {"\033[A", 3};
```
and 
```C
manipulate_terminal(STDOUT_FILENO, &CLEAR_SCREEN);
```
Oh - also yes! that does mean that I can now clear the screen too!


### 😵‍💫 Storing and Saving Text
Ok, so now we need to store the data.
**No problem** — we'll just chuck all the characters into a single array, right? ...

 Right?

Well, then what if the user edits just one line?  
Do we rewrite the entire file? That seems wasteful.  
**Ok fine**, let’s split the content into lines and store each line as its own array.  
That way, someday in the future, we can just overwrite lines that were changed. Cool, yes that is a sounds like a good idea.

We’ll create a `Line` structure that holds the characters, the current length, and the maximum 
length it can be. And then we’ll have a `ContentManager` (because naming things is free) that holds 
an array of lines, the current line, and a maximum number of lines. It might fill up — but we’ll 
cross that bridge when we get there.

Now: when we read a new line, we need to:

1. Check that there’s space for two more characters (`\n` and `\0`)
2. Add the newline and null-terminator
3. Increment the line count
4. Initialize the next line’s structure

A bit of work, but doable.  
Actually — wait — we need to check if we even *have* enough lines left in the buffer.

**Hmmmm.** Ok.  
**No no. This is fine.**

Then to insert a character we:

1. Check if there's space in the current line
2. If there is, great — we add it and increase the length
3. If not, we terminate the line and move to the next one

Wait.

This is getting messy.  
There are pointers everywhere.  
And I’m slightly panicking about memory.  
And lines.  
And—

**HOLD ON ONE SECOND.**

What if the user wants to insert into the *middle* of the line? 

**AAhjhhajahabbhaaa**

There has to be a better way to do this, right?

Feeling **very demotivated** and just a little bit broken, I did what any desperate person in 2025 
would do – I cracked turned to AI.  

But I didn’t ask for code. I just explained the problem.  

It basically said,
"Yep. That’s text editing in C. Good luck, buddy."
OK, thanks ChatGPT — let’s go ask Google.

Turns out that storing and manipulating text is actually... kind of a hard problem.
And I thought this would be a quick little project 😅.


Anyway I found [this article by Cameron DaCamara](https://cdacamar.github.io/data%20structures/algorithms/benchmarking/text%20editors/c++/editor-data-structures/) which describes exactly what I’ve been going through — 
just maybe with a  more professional tone.

So at this point, I made a decision:
Get the current implementation working first — even if inserting in the middle of a line just 
triggers overtype Mode (Sorry ... Not sorry).

It was also around here that I thought maybe I should map out how the different parts of the code 
interact. That gave me this:

![V0.1.0-block diagram](./assets/figs/Wysig-v0.1.0.svg)

Now, the arrow keys aren’t working properly. When I move the cursor around, the saved file ends up 
with missing or jumbled lines.

Right — so, what if the `ContentManager` tracks the cursor position, and the `ScreenManager` just 
asks it where to place the curser?

That way I don’t have to duplicate logic between the input manager and the content manager.
Also, that logic probably shouldn’t have been in the input manager anyway.

So the system should probably look like this instead:

![V0.2.0-block diagram](./assets/figs/Wysig-v0.2.0.svg)

Ahhh much cleaner.

---
So after having this idea, and with a basic editor working (I mean it has some quirks) , I tagged 
that version as v0.1.0 and decided to start refactoring everything.

Oh — and somewhere in the middle of this, I wrote a Python file in Wysig and successfully ran it in 
the terminal. That moment felt kinda huge.

What has become of my life 🙈 



