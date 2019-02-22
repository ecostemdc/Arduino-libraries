/*
  ArduCAM.cpp - Arduino library support for CMOS Image Sensor
  Copyright (C)2011-2013 ArduCAM.com. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ArduCAM.com. You can find the latest version of the library at
  http://www.ArduCAM.com

  Now supported controllers:
		-	OV7670
		-	MT9D111
		-	OV7675
		-	OV2640
		-	OV3640
		-	OV5642
		-	OV7660
		-	OV7725			
			
	We will add support for many other sensors in next release.
        
  Supported MCU platform
 		-	Theoretically support all Arduino families
  		-	Arduino UNO R3			(Tested)
  		-	Arduino MEGA2560 R3		(Tested)
  		-	Arduino Leonardo R3		(Tested)
  		-	Arduino Nano			(Tested)
  		-	Arduino DUE				(Tested)
  		- Arduino Yun				(Tested)
  		-	Raspberry Pi			(In plan)
  		      
  If you make any modifications or improvements to the code, I would appreciate
  that you share the code with me so that I might include it in the next release.
  I can be contacted through http://www.ArduCAM.com

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

/*------------------------------------
	Revision History:
	2012/09/20 	V1.0.0	by Lee	first release 	
	2012/10/23  V1.0.1  by Lee  Resolved some timing issue for the Read/Write Register
	2012/11/29	V1.1.0	by Lee  Add support for MT9D111 sensor
	2012/12/13	V1.2.0	by Lee	Add support for OV7675 sensor
	2012/12/28  V1.3.0	by Lee	Add support for OV2640,OV3640,OV5642 sensors
	2013/02/26	V2.0.0	by Lee	New Rev.B shield hardware, add support for FIFO control 
															and support Mega1280/2560 boards 
	2013/05/28	V2.1.0	by Lee	Add support all drawing functions derived from UTFT library 		
	2013/08/24	V3.0.0	by Lee	Support ArudCAM shield Rev.C hardware, features SPI interface and low power mode.
								Support almost all series of Arduino boards including DUE.
	2014/02/06  V3.0.1  by Lee  Minor change to the library, fixed some bugs, add self test code to the sketches for easy debugging.
	2014/03/09  V3.1.0  by Lee  Add the more impressive example sketches. 
								Optimise the OV5642 settings, improve image quality.
								Add live preview before JPEG capture.
								Add play back photos one by one	after BMP capture.	
	2014/05/01  V3.1.1  by Lee  Minor changes to add support Arduino IDE for linux distributions.				
	2014/09/30  V3.2.0  by Lee  Improvement on OV5642 camera dirver.			
	2014/10/06  V3.3.0  by Lee  Add OV7660,OV7725 camera support.	
	2015/02/27  V3.4.0  by Lee  Add the support for Arduino Yun board, update the latest UTFT library for ArduCAM.																	
--------------------------------------*/
#include "Arduino.h"
#include "ov7670.h"
#include "ov7670_regs.h"
#include <Wire.h>




ArduCAM::ArduCAM()
{
	sensor_model = OV7670;
	sensor_addr = 0x42;
}

ArduCAM::ArduCAM(byte model,int CS)
{ 
	
	
	sensor_model=OV7670;
  sensor_addr = 0x42;
}

		

byte ArduCAM::wrSensorReg8_8(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID & 0x00FF); 	
	Wire.write(regDat & 0x00FF); 	

	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  	return(1);
}

byte ArduCAM::rdSensorReg8_8(uint8_t regID, uint8_t* regDat)
{

	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID & 0x00FF); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),1);
	if(Wire.available())
		*regDat = Wire.read(); 	

	delay(1);
  	return(1);
}

byte ArduCAM::wrSensorReg8_16(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID & 0x00FF); 	

	Wire.write(regDat >> 8);            // sends data byte, MSB first
  	Wire.write(regDat & 0x00FF);  	
	
	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  	return(1);
}

byte ArduCAM::rdSensorReg8_16(uint8_t regID, uint16_t* regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),2);
	if(Wire.available())
	{
		*regDat = Wire.read(); 	
		*regDat++ = Wire.read();
	}

	delay(1);
  	return(1);
}

byte ArduCAM::wrSensorReg16_8(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID >> 8);            // sends instruction byte, MSB first
  	Wire.write(regID & 0x00FF); 	
  	Wire.write(regDat & 0x00FF);  	

	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  	return(1);
}


byte ArduCAM::rdSensorReg16_8(uint16_t regID, uint8_t* regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID >> 8);
	Wire.write(regID & 0x00FF); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),1);
	if(Wire.available())
	{
		*regDat = Wire.read(); 	
	}
	delay(1);
  	return(1);
}

byte ArduCAM::wrSensorReg16_16(int regID, int regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	
	Wire.write(regID >> 8);            // sends instruction byte, MSB first
  	Wire.write(regID & 0x00FF); 	

  	Wire.write(regDat >> 8);            // sends data byte, MSB first
  	Wire.write(regDat & 0x00FF);  	

	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  	return(1);
}

byte ArduCAM::rdSensorReg16_16(uint16_t regID, uint16_t* regDat)
{
	Wire.beginTransmission(sensor_addr >> 1);
	Wire.write(regID >> 8);
	Wire.write(regID & 0x00FF); 	
	Wire.endTransmission();
	
	Wire.requestFrom((sensor_addr >> 1),2);
	if(Wire.available())
	{
		*regDat = Wire.read(); 	
		*regDat++ = Wire.read(); 	
	}

	delay(1);
  	return(1);
}


int ArduCAM::wrSensorRegs8_8(const struct sensor_reg reglist[])
{
	int err = 0;
	uint16_t reg_addr = 0;
	uint16_t reg_val = 0;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xff) | (reg_val != 0xff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg8_8(reg_addr, reg_val);

		//if (!err)
		//{
		//	return err;
		//}
	   	next++;
	   	
	} 
	
	return 1;
}


int ArduCAM::wrSensorRegs8_16(const struct sensor_reg reglist[])
{
	int err = 0;
	
	unsigned int reg_addr,reg_val;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xff) | (reg_val != 0xffff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg8_16(reg_addr, reg_val);
		//	if (!err)
	   	//return err;
	   	next++;
	}  
	
	return 1;
}

int ArduCAM::wrSensorRegs16_8(const struct sensor_reg reglist[])
{
	int err = 0;
	
	unsigned int reg_addr;
	unsigned char reg_val;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xffff) | (reg_val != 0xff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg16_8(reg_addr, reg_val);
		//if (!err)
	   	//return err;
	   next++;
	} 
	
	return 1;
}

int ArduCAM::wrSensorRegs16_16(const struct sensor_reg reglist[])
{
	int err = 0;
	
	unsigned int reg_addr,reg_val;
	const struct sensor_reg *next = reglist;
	
	while ((reg_addr != 0xffff) | (reg_val != 0xffff))
	{		
		reg_addr = pgm_read_word(&next->reg);
		reg_val = pgm_read_word(&next->val);
		err = wrSensorReg16_16(reg_addr, reg_val);
		//if (!err)
	    //   return err;
	   next++;
	} 
	
	return 1;
}

			
void ArduCAM::InitCAM()
{
	byte rtn = 0;
	byte reg_val;
  
	wrSensorReg8_8(0x12, 0x80);
	delay(100);
	rtn = wrSensorRegs8_8(OV7670_QVGA);
  
}
