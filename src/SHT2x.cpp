/*
  SHT2x - A Humidity Library for Arduino.

  Supported Sensor modules:
    SHT21-Breakout Module - http://www.moderndevice.com/products/sht21-humidity-sensor
	SHT2x-Breakout Module - http://www.misenso.com/products/001
	
  Created by Christopher Ladden at Modern Device on December 2009.
  Modified by Paul Badger March 2010
  
  Modified by www.misenso.com on October 2011:
	- code optimisation
	- compatibility with Arduino 1.0
  
  Modified by (@amontero) cicytex.juntaex.es on April 2023
  These modification are related to new versions of Wire lirary
  - added delay after transmission end
  - added setter for delay

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <inttypes.h>
#include <Wire.h>
#include "Arduino.h"
#include "SHT2x.h"



/******************************************************************************
 * Global Functions
 ******************************************************************************/

SHT2xClass::SHT2xClass()
{
	measTempReady = false;
	measHumReady = false;
	measuringTemp = false;
	measuringHum = false;
}	
 
/**********************************************************
 * GetHumidity
 *  Gets the current humidity from the sensor.
 *
 * @return float - The relative humidity in %RH
 **********************************************************/
float SHT2xClass::GetHumidity(void)
{
	return (-6.0 + 125.0 / 65536.0 * (float)(readSensor(eRHumidityHoldCmd)));
}

boolean SHT2xClass::GetHumidityNoHold(float *h)
{
	uint16_t a;

	if(measuringHum == true){
		a = readSensorNoHoldRead();
		if(measHumReady){
			*h = (-6.0 + 125.0 / 65536.0 * (float)(a));
			return(true);
		}else
			return(false);
	}else{
		return(false);
	}
}

/**********************************************************
 * GetTemperature
 *  Gets the current temperature from the sensor.
 *
 * @return float - The temperature in Deg C
 **********************************************************/
float SHT2xClass::GetTemperature(void)
{
	return (-46.85 + 175.72 / 65536.0 * (float)(readSensor(eTempHoldCmd)));
}

boolean SHT2xClass::GetTemperatureNoHold(float *t)
{
	uint16_t a;

	if(measuringTemp == true){
		a = readSensorNoHoldRead();
		if(measTempReady){
			*t = (-46.85 + 175.72 / 65536.0 * (float)(a));
			return(true);
		}else
			return(false);
	}else{
		return(false);
	}
}


/******************************************************************************
 * Private Functions
 ******************************************************************************/

uint16_t SHT2xClass::readSensor(uint8_t command)
{
    uint16_t result;

    Wire.beginTransmission(eSHT2xAddress);	//begin
    Wire.write(command);					//send the pointer location
    Wire.endTransmission();               	//end
	delay(_msecs);
    Wire.requestFrom(eSHT2xAddress, 3);
    while(Wire.available() < 3) {
      ; //wait
    }

    //Store the result
    result = ((Wire.read()) << 8);
    result += Wire.read();
	result &= ~0x0003;   // clear two low bits (status bits)
    return result;
}


void SHT2xClass::readSensorNoHold(uint8_t command){
	uint16_t result;

    Wire.beginTransmission(eSHT2xAddress);	//begin
    Wire.write(command);					//send the pointer location
    Wire.endTransmission();               	//end
	delay(_msecs);
}


uint16_t SHT2xClass::readSensorNoHoldRead(void){
	uint16_t result;
	
    Wire.requestFrom(eSHT2xAddress, 3);
    if(Wire.available() < 3) return -1;
      
    //Store the result
    result = ((Wire.read()) << 8);
    result += Wire.read();
	if(bitRead(result,1) == 0){
		measTempReady = true;
		measuringTemp = false;		
	}else{
		measHumReady = true;
		measuringHum = false;
	}
	
	result &= ~0x0003;   // clear two low bits (status bits)
    return result;
	
}

void SHT2xClass::PrepareTemperatureNoHold(void){
	measTempReady = false;
	measuringTemp = true;
	readSensorNoHold(eTempNoHoldCmd);
}

void SHT2xClass::PrepareHumidityNoHold(void){
	measHumReady = false;
	measuringHum = true;
	readSensorNoHold(eRHumidityNoHoldCmd);
}

void SHT2xClass::setEndTransmissionTime(uint32_t msecs){
 	_msecs = msecs;
}

