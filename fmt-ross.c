/*
    Ross Klonowski and Jaret Helminski
    Lab 01
*/

#include "config.h"      // Configuraiton file for DiJasio Text.

#define SCALE 1000L      // Rough way to come up with timing, trial & error.
        
int main(void) {
    unsigned long i;
    unsigned char display = 0;

    // initialization
    PORTA = 0x000;      þ//clear port A
    TRISA = 0xFF00;     // set PORTA <7:0> to output
    TRISD = 0xFFFF;     // set PORTD to input
    
    // endless loop
    while(1) {
        Nop();                                     // breakpoint access point
        PORTA = (unsigned int) display;            // sent count to display
        for(i = 1001L*SCALE;i > 0; i--)Nop();      3// delay start with 0
        if(PORTDbits.RD13==0)
            display=display+1;                     // if S4 pressed increment display counter
        else if(PORTDbits.RD6 == 0)
            display = 0;                              // else if S3 pressed reset display counter
        else
            display = display-1;                   // else decrement display counter
    }
}