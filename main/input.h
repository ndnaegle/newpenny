// input.h
//  Debounce the input with a variable delay
//  June 2017

#ifndef _INPUT_H
#define _INPUT_H

class Input {
private:
    unsigned long int m_lastDebounceTime;
    int m_lastButtonState;
    int m_pin;
    int m_debounceDelay;
    char m_displaySymbol;
public:
    Input(int pin, int delay, char displaySymbol); // delay is typically 40-50ms
    void start(void);   // start the debouncing timer
    int read(void);     // returns the value or -1 if we haven't waited long enough yet
    int readBouncy(void); // reads the raw value
    void displayLevel(void); // displays the pin's level on the OLED
};

#define MAX_PINS 10

extern Input inputHome;
extern Input input90Degree;
extern Input inputCoinAccept;
extern Input inputButton1;
extern Input inputButton2;
extern Input inputButton3;
extern Input inputButton4;

extern void initSerialKeys(void);
extern void updateSerialKeys(void);

#endif // #ifndef _INPUT_H
