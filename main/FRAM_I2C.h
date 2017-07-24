// FRAM_I2C.h
//  Heavily modified version of adafruit FRAM module

#ifndef _FRAM_I2C_H_
#define _FRAM_I2C_H_

#include <Arduino.h>
#include <Wire.h>

#define MB85RC_DEFAULT_ADDRESS (0x50) /* 1010 + A2 + A1 + A0 = 0x50 default */
#define MB85RC_SLAVE_ID (0xF8)

class FRAM_I2C {
    
public:
    FRAM_I2C(void);
    boolean begin(uint8_t addr = MB85RC_DEFAULT_ADDRESS);
    void write8 (uint16_t framAddr, uint8_t value);
    uint8_t read8  (uint16_t framAddr);
    void erase(void);
    uint16_t offset(uint16_t addr, uint8_t offset);
    uint16_t m_density;
    uint16_t m_maxaddress;

private:
    uint8_t m_i2c_addr;
    uint8_t m_manufacturerID;
    uint8_t m_densityCode;
    uint8_t m_productID;
    boolean m_framInitialized;
};

#endif
