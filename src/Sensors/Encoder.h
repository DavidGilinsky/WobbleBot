/*
Encoder driver code is placed under the BSD license.
Copyright (c) 2014, Emlid Limited, www.emlid.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Emlid Limited nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL EMLID LIMITED BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef Encoder_HPP
#define Encoder_HPP

#include "../Navio/I2Cdev.h"
#include <math.h>
#include <unistd.h>
#include <string>

#define Encoder_ADDRESS_CSB_LOW  0x76
#define Encoder_ADDRESS_CSB_HIGH 0x29
#define Encoder_DEFAULT_ADDRESS  Encoder_ADDRESS_CSB_HIGH

#define Encoder_RA_STATUS           0x00
#define Encoder_RA_RESET         0x0B

#define Encoder_WD_1		 0x10
#define Encoder_WD_2		 0x14

#define Encoder_RA_C0            0xA0
#define Encoder_RA_C1            0xA2
#define Encoder_RA_C2            0xA4
#define Encoder_RA_C3            0xA6
#define Encoder_RA_C4            0xA8
#define Encoder_RA_C5            0xAA
#define Encoder_RA_C6            0xAC
#define Encoder_RA_C7            0xAE

#define Encoder_RA_D1_OSR_256    0x40
#define Encoder_RA_D1_OSR_512    0x42
#define Encoder_RA_D1_OSR_1024   0x44
#define Encoder_RA_D1_OSR_2048   0x46
#define Encoder_RA_D1_OSR_4096   0x48

#define Encoder_RA_D2_OSR_256    0x50
#define Encoder_RA_D2_OSR_512    0x52
#define Encoder_RA_D2_OSR_1024   0x54
#define Encoder_RA_D2_OSR_2048   0x56
#define Encoder_RA_D2_OSR_4096   0x58

class Encoder {
    public:
	    Encoder(uint8_t address = Encoder_DEFAULT_ADDRESS);

        void initialize();
        bool testConnection();

	    void resetPosition(uint8_t OSR = Encoder_RA_RESET);
	    void readWheel_2();

	    void refreshTemperature(uint8_t OSR = Encoder_RA_D2_OSR_4096);
	    void readWheel_1();

	    void calculatePressureAndTemperature();
	    void update();

	    long getWheel_1();
	    long getWheel_2();

    private:
	    uint8_t devAddr; // I2C device adress
	    uint16_t C1, C2, C3, C4, C5, C6; // Calibration data
	    long D1, D2; // Raw measurement data
	    float TEMP; // Calculated temperature
	    float PRES; // Calculated pressure
};

#endif // Encoder_HPP
