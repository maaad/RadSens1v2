#ifndef _RADSENS1V2_H_
#define _RADSENS1V2_H_

#include <stdint.h>

#include <Arduino.h>
#include <Wire.h>


#ifndef _DEFINES_H_
#define _DEFINES_H_
#define RS_DEFAULT_I2C_ADDRESS 0x66
#define RS_DEVICE_ID_RG 0x00
#define RS_FIRMWARE_VER_RG 0x01
#define RS_RAD_INTENSY_DYNAMIC_RG 0x03
#define RS_RAD_INTENSY_STATIC_RG 0x06
#define RS_PULSE_COUNTER_RG 0x09
#define RS_DEVICE_ADDRESS_RG 0x10
#define RS_HV_GENERATOR_RG 0x11
#define RS_SENSITIVITY_RG 0x12
#define RS_LED_CONTROL_RG 0x14
#endif

class ClimateGuard_RadSens1v2
{
private:

    uint8_t _sensor_address;
    uint8_t _data[19] = {0};
    uint8_t _chip_id;
    uint8_t _firmware_ver;
    uint32_t _pulse_cnt;
    bool updateData();

public:
    ClimateGuard_RadSens1v2();
    ~ClimateGuard_RadSens1v2();

    //Fields of data, for update use getData() function
    float intensy_static = 0;
    float intensy_dyanmic = 0;
    uint32_t pulses = 0;

    //Methods for get or set data
    bool radSens_init();
    bool getData();
    uint8_t getChipId();
    uint8_t getFirmwareVersion();
    float getRadIntensyDynamic();
    float getRadIntensyStatic();
    uint32_t getNumberOfPulses();
    uint8_t getSensorAddress();
    bool getHVGeneratorState();
    bool getLedState();
    uint16_t getSensitivity();
    bool setHVGeneratorState(bool state);
    bool setSensitivity(uint16_t sens);
    bool setLedState(bool state);
};

#endif // _RADSENS1V2_H_

