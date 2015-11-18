#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
/* Set Pin Numbers */
#define BUTTONRED 12
#define BUTTONGREEN 13
#define BUTTONBLUE 14
#define INCREASE 24
#define DECREASE 25
#define RED 27
#define GREEN 28
#define BLUE 29

int main(void) {
    /* Initialize GPIO pins to map to wiringPi numbers */
    if(wiringPiSetup() == -1)
    return 1; //return with error status (initialization failed)
    
    /* Set modes for the GPIO pins */
    pinMode(BUTTONRED, INPUT);
    pinMode(BUTTONGREEN, INPUT);
    pinMode(BUTTONBLUE, INPUT);
    pinMode(INCREASE, INPUT);
    pinMode(DECREASE, INPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

     /* save the value of all LED*/
    int valueRed = 0;
    int valueGreen = 0;
    int valueBlue = 0;
    
    /* create controller*/
    softPwmCreate(RED, valueRed, 100);
    softPwmCreate(GREEN, valueGreen, 100);
    softPwmCreate(BLUE, valueBlue, 100);

    /* initialize input */
    int red = 0;
    int green = 0;
    int blue = 0;
    int increase = 0;
    int decrease = 0;
    
    for(;;) {
        /* read from buttom*/
        red = digitalRead(BUTTONRED);
        green = digitalRead(BUTTONGREEN);
        blue = digitalRead(BUTTONBLUE);
        increase = digitalRead(INCREASE);
        decrease = digitalRead(DECREASE);
        
        /* Change the LED value */
        if (red) {
            printf("1");
            if (increase) valueRed += 5;
            else if (decrease) valueRed -=5;
            if (valueRed > 100) valueRed = 100;
            if (valueRed < 0) valueRed = 0;
            softPwmWrite(RED, valueRed);
        }
        if (green) {
            printf("2");
            if (increase) valueGreen += 5;
            else if (decrease) valueGreen -=5;
            if (valueGreen > 100) valueGreen = 100;
            if (valueGreen < 0) valueGreen = 0;
            softPwmWrite(GREEN, valueGreen);
        }
        if (blue) {
            printf("3");
            if (increase) valueBlue += 5;
            else if (decrease) valueBlue -=5;
            if (valueBlue > 100) valueBlue = 100;
            if (valueBlue < 0) valueBlue = 0;
            softPwmWrite(BLUE, valueBlue);
        }
        delay(100); 
    }
    return 0;
}