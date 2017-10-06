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

#include "Encoder.h"

/** Encoder constructor.
 * @param address I2C address
 * @see Encoder_DEFAULT_ADDRESS
 */
Encoder::Encoder(uint8_t address) {
    this->devAddr = address;
}

/** Power on and prepare for general usage.
 * This method reads coefficients stored in PROM.
 */
void Encoder::initialize() {
    // Reading 6 calibration data values
    uint8_t buff[2];
    I2Cdev::readBytes(devAddr, Encoder_RA_C1, 2, buff);
    C1 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, Encoder_RA_C2, 2, buff);
    C2 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, Encoder_RA_C3, 2, buff);
    C3 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, Encoder_RA_C4, 2, buff);
    C4 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, Encoder_RA_C5, 2, buff);
    C5 = buff[0]<<8 | buff[1];
    I2Cdev::readBytes(devAddr, Encoder_RA_C6, 2, buff);
    C6 = buff[0]<<8 | buff[1];

}

/** Verify the I2C connection.
 * @return True if connection is valid, false otherwise
 */
bool Encoder::testConnection() {
    uint8_t data;
    int8_t status = I2Cdev::readByte(devAddr, Encoder_RA_STATUS, &data);
    if (status > 0)
        return true;
    else
        return false;
}

/** Initiate the process of pressure measurement
 * @param OSR value
 * @see Encoder_RA_D1_OSR_4096
 */
void Encoder::resetPosition(uint8_t OSR) {
    I2Cdev::writeByte(devAddr, Encoder_RA_RESET, 1);
}

/** Read pressure value
 */
void Encoder::readWheel_2() {
    //
    uint8_t buffer[4];
    if(I2Cdev::readBytes(devAddr, Encoder_WD_1, 4, buffer) != -1){
    	D1 = (buffer[3] << 24) | (buffer[2] << 16) | buffer[1] << 8 | buffer[0];
    }
//	D1=buffer[3];
}

/** Initiate the process of temperature measurement
 * @param OSR value
 * @see Encoder_RA_D2_OSR_4096
 */
void Encoder::refreshTemperature(uint8_t OSR) {
	I2Cdev::writeBytes(devAddr, OSR, 0, 0);
}

/** Read temperature value
 */
void Encoder::readWheel_1() {
	uint8_t buffer[4];

	if(I2Cdev::readBytes(devAddr, Encoder_WD_2, 4, buffer) != -1){
		D2 = (buffer[3] << 24) | (buffer[2] << 16) | buffer[1] << 8 | buffer[0];
	}
}


/** Get calculated temperature value
 @return Temperature in degrees of Celsius
 */
long Encoder::getWheel_1() {
	readWheel_1();
    return D1;
}

/** Get calculated pressure value
 @return Pressure in millibars
 */
long Encoder::getWheel_2() {
	readWheel_2();
	return D2;
}
