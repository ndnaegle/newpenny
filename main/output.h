// output.h
//  Keep track of output pin state
//  July 2017

#ifndef _OUTPUT_H
#define _OUTPUT_H

class Output {
private:
    int m_pin;
    int m_value;
    const char* m_name;
    char m_displaySymbol;
public:
    Output(int pin, const char* name, char displaySymbol);
    void write(int value);
    int readback(void);
    void displayLevel(void);
};

extern Output outputCounter;
extern Output outputCoinDrop;
extern Output outputMotorRun;
extern Output outputRedLeverLift;

#endif // #ifndef _OUTPUT_H
