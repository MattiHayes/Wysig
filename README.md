# Wysig

**Wysig** is a terminal based text editor written in C. 

This project was inspired by [kilo](https://viewsourcecode.org/snaptoken/kilo/index.html). I have looked at parts of the kilo tutorial however, my goal is to build this as independently as possible. I will write my own code  mostly from scratch. Google is my first stop when I get stuck and I use AI as a sounding board - not as a code generator. 

## TODO:
```
[ ] Read up on termios.h
[ ] Screen
    [ ] update the screen
    [ ] Clean the screen and update it
[ ] Inputs
    [ ] Filter inputs
    [ ] Display input letters on the screen
    [ ] Store inputs
[ ] (When done) update README to past tense

```

## Explanation of Parts
### 🥩 'Raw' Mode 
So the first step was to disable 'cooked' mode where the terminal echos what has been entered from 
the keyboard. This required the use of `termios.h`.
