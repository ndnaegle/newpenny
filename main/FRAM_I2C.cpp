// FRAM_I2C.cpp
//  Heavily modified from adafruit version
#include <stdlib.h>
#include <math.h>

#include "FRAM_I2C.h"

FRAM_I2C::FRAM_I2C(void):
    m_framInitialized(false)
{
}

boolean FRAM_I2C::begin(uint8_t addr)
{
    m_i2c_addr = addr;
    Wire.begin();
    
    Wire.beginTransmission(MB85RC_SLAVE_ID >> 1);
    Wire.write(m_i2c_addr << 1);
    Wire.endTransmission(false);
    
    Wire.requestFrom(MB85RC_SLAVE_ID >> 1, 3);
    uint8_t a[3] = { 0, 0, 0 };
    a[0] = Wire.read();
    a[1] = Wire.read();
    a[2] = Wire.read();
    
    m_manufacturerID = (a[0] << 4) + (a[1] >> 4);
    m_densityCode = (uint8_t)(a[1] & 0x0F);
    m_productID = ((a[1] & 0x0F) << 8) + a[2];
    
    if (m_manufacturerID == 0x0a) { // Fujitsu
        switch (m_densityCode) {
        case 0x00: // 4K
            m_density = 4;
            m_maxaddress = 511;
            break;
        case 0x03: // 64K
            m_density = 64;
            m_maxaddress = 8191;
            break;
        case 0x05: // 256K
            m_density = 256;
            m_maxaddress = 32767;
            break;
        case 0x06: // 512K
            m_density = 512;
            m_maxaddress = 65535;
            break;
        default:
            m_density = 0;
            m_maxaddress = 0;
            Serial.print(F("Unrecognized Fujitsu device"));
            return false;
        }
    } else if (m_manufacturerID == 0x04) { // Cypress
        switch (m_densityCode) {
        case 0x01: // 128K
            m_density = 128;
            m_maxaddress = 16383;
            break;
        case 0x02: // 256K
            m_density = 256;
            m_maxaddress = 32767;
            break;
        case 0x03: // 512K
            m_density = 512;
            m_maxaddress = 65535;
            break;
        default:
            m_density = 0;
            m_maxaddress = 0;
            Serial.print(F("Unrecognized Cypress device"));
            return false;
        }
    }
//    Serial.print(F("Manufacturer ID: 0x"));
//    Serial.println(m_manufacturerID, HEX);
//    Serial.print(F("Product ID: 0x"));
//    Serial.println(m_productID, HEX);
//    Serial.print(F("densityCode: 0x"));
//    Serial.println(m_densityCode, HEX);
//    Serial.print(F("density: "));
//    Serial.println(m_density, DEC);
//    Serial.print(F("maxaddress: "));
//    Serial.println(m_maxaddress, DEC);

    m_framInitialized = true;

    return true;
}

void FRAM_I2C::write8 (uint16_t framAddr, uint8_t value)
{
    Wire.beginTransmission(m_i2c_addr);
    Wire.write(framAddr >> 8);
    Wire.write(framAddr & 0xFF);
    Wire.write(value);
    Wire.endTransmission();
}

uint8_t FRAM_I2C::read8 (uint16_t framAddr)
{
    Wire.beginTransmission(m_i2c_addr);
    Wire.write(framAddr >> 8);
    Wire.write(framAddr & 0xFF);
    Wire.endTransmission();
    
    Wire.requestFrom(m_i2c_addr, (uint8_t)1);
    
    return Wire.read();
}

uint16_t FRAM_I2C::offset(uint16_t addr, uint8_t offset) {
    uint32_t bigaddr = (uint32_t)addr + offset;
    if (bigaddr > (uint32_t)m_maxaddress) {
        bigaddr -= (uint32_t)m_maxaddress + 1; // 32767 + 2 = 32769. 32769 - (32767 + 1) = 1 (wraps around)
    }

    return (uint16_t)bigaddr;
}

void FRAM_I2C::erase(void) {
    for (uint16_t i = 0 ; i < m_maxaddress ; ++i) {
        write8(i, 0x00);
    }
}

