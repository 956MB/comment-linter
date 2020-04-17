# comment-linter

Comment "linter" vertically lines up groups of comments inside a file.

_Yes, I'm sure something like this already exists, and is written in a much better way, but oh well. I thought it was a cool idea._

### How:

Commments in a file are first grouped. Groups are made up of comments that are within 1 line space of each other. Next, out of these groups, the furthest to the right comment is picked, as to line up the rest of the comments in a group with it.

"top level" comments are when there is nothing but blank spaces before the comment, and these are not counted into groups, and are kept where they are. An example of this could be a comment saying what the function below it does, or a chunk of code that was commmented out. That way the chunks indentation will not be messed up, because all that will be in front of it is blank spaces.

### What it does not do:

- Fix extra or missing spaces inside comments.
- Horizontally fix comments locations to the tab size in the file.
    - If the furthest comment in a group is 6 spaces to the right of code, all this will do is line up all other comments in the group to this comment. Not move the comments in to the current tab size, say 4 spaces. Maybe this can be done pretty easily, idk.

Before:

```c++
int main(void) {
    unsigned long i;
    unsigned char display = 0;

    // initialization                                       << "top level" comment with all blank spaces does not get grouped
    PORTA = 0x000;  // clear port A                         <<
    TRISA = 0xFF00;     // set PORTA <7:0> to output        << Comments seperated by 0 or 1 line spaces are grouped
    TRISD = 0xFFFF;     // set PORTD to input               <<

    // ...                                                  << 2 or more line spaces split up the groups

    // endless loop
    while(1) {
        Nop();          // breakpoint access point
        PORTA = (unsigned int) display; // sent count to display
        for(i = 1001L*SCALE;i > 0; i--)Nop();      // delay start with 0
        if(PORTDbits.RD13==0)
            display=display+1;          // if S4 pressed increment display counter
        else if(PORTDbits.RD6 == 0)
            display = 0;   // else if S3 pressed reset display counter
        else
            display = display-1;        // else decrement display counter
    }
```

After:

```c++
int main(void) {
    unsigned long i;
    unsigned char display = 0;

    // initialization
    PORTA = 0x000;      // clear port A
    TRISA = 0xFF00;     // set PORTA <7:0> to output
    TRISD = 0xFFFF;     // set PORTD to input

    // endless loop
    while(1) {
        Nop();                                     // breakpoint access point
        PORTA = (unsigned int) display;            // sent count to display
        for(i = 1001L*SCALE;i > 0; i--)Nop();      // delay start with 0
        if(PORTDbits.RD13==0)
            display=display+1;                     // if S4 pressed increment display counter
        else if(PORTDbits.RD6 == 0)
            display = 0;                           // else if S3 pressed reset display counter
        else
            display = display-1;                   // else decrement display counter
    }
}
```
