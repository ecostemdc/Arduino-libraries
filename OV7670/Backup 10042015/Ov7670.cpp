/*
 * Ov7670.cpp
 *
 *  Created on: Aug 26, 2013
 *      Author: arndtjenssen
 */

#include <Arduino.h>
#include "Ov7670.h"
#include "Wire.h"


Ov7670::Ov7670() {
	init_success = false;
}


void Ov7670::nightMode(bool enable) {
	if (enable) {
		I2C_write(OV7670_I2C_ADDR, REG_COM11, COM11_EXP|COM11_HZAUTO);
		I2C_write(OV7670_I2C_ADDR, REG_COM11, COM11_EXP|COM11_HZAUTO|COM11_NIGHT|COM11_NIGHT_FR8);
	} else {
		I2C_write(OV7670_I2C_ADDR, REG_COM11, COM11_EXP|COM11_HZAUTO|COM11_NIGHT|COM11_NIGHT_FR8);
		I2C_write(OV7670_I2C_ADDR, REG_COM11, COM11_EXP|COM11_HZAUTO);
	}
}

// -2 (low contrast ) to +2 (high contrast)
void Ov7670::contrast(int8_t value) {
	static const uint8_t values[] = {0x60, 0x50, 0x40, 0x38, 0x30};

	value = min(max((value + 2), 0), 4);
	I2C_write(OV7670_I2C_ADDR, REG_CONTRAST, values[value]);
}

// -2 (dark) to +2 (bright)
void Ov7670::brightness(int8_t value) {
	static const uint8_t values[] = {0xb0, 0x98, 0x00, 0x18, 0x30};

	value = min(max((value + 2), 0), 4);
	I2C_write(OV7670_I2C_ADDR, REG_BRIGHT, values[value]);
}


uint8_t Ov7670::I2C_write(uint8_t slave_address, uint8_t address, uint8_t data) 
{
	Wire.beginTransmission(slave_address >> 1);
	Wire.write(address); 	
	Wire.write(data); 	

	if(Wire.endTransmission())
	{
		return 0; 
	}

	delay(1);
  	return(1);

}

uint8_t Ov7670::I2C_read(uint8_t slave_address, uint8_t address, uint8_t *data) 
{

	Wire.beginTransmission(slave_address >> 1);
	Wire.write(address); 	
	Wire.endTransmission();
	
	Wire.requestFrom( slave_address>>1 ,1);
	if(Wire.available())
		*data = Wire.read(); 	

	delay(1);
  	return(1);
}




uint8_t Ov7670::transfer_regvals(const struct regval_list *list)
{
	int err = 0;
	
	unsigned char reg_addr;
	unsigned char reg_val;
	const struct regval_list *next = list;
	
	while ((reg_addr != EM) || (reg_val != EM))
	{		
		reg_addr = pgm_read_word(&next->reg_num);
		reg_val = pgm_read_word(&next->value);
/*    
 Serial.print("Add : "); 
 Serial.print(reg_addr,HEX); 
 Serial.print(" - "); 
 Serial.println(reg_val,HEX); 
*/    
    I2C_write(OV7670_I2C_ADDR, reg_addr, reg_val);

		// delay for reset command
		if ((reg_addr == REG_COM7) && (reg_val == COM7_RESET)) 
    {
			_delay_ms(200);
    }
		//if (!err)
	   	//return err;
	   next++;
	} 
	
	
	return 1;
}



uint8_t Ov7670::init_rgb444_qqvga() {
	uint8_t ret = 0;

	I2C_write(OV7670_I2C_ADDR, REG_COM7, COM7_RGB | COM7_QQVGA);
	ret = transfer_regvals(ov7670_fmt_rgb444);
	if (ret != 1) return ret;

	return transfer_regvals(ov7670_qqvga);
}

uint8_t Ov7670::init_rgb555_qqvga() {
	uint8_t ret = 0;

	I2C_write(OV7670_I2C_ADDR, REG_COM7, COM7_RGB | COM7_QQVGA);
	ret = transfer_regvals(ov7670_fmt_rgb555);
	if (ret != 1) return ret;

	return transfer_regvals(ov7670_qqvga);
}

uint8_t Ov7670::init_rgb565_qqvga() {
	uint8_t ret = 0;

	I2C_write(OV7670_I2C_ADDR, REG_COM7, COM7_RGB | COM7_QQVGA | COM7_COLOR_BAR);
	ret = transfer_regvals(ov7670_fmt_rgb565);
	if (ret != 1) return ret;

	return transfer_regvals(ov7670_qqvga);
}

uint8_t Ov7670::init_yuv_qqvga() {
	uint8_t ret = 0;

	I2C_write(OV7670_I2C_ADDR, REG_COM7, COM7_YUV);
	ret = transfer_regvals(ov7670_fmt_yuv422);
	if (ret != 1) return ret;

	return transfer_regvals(ov7670_qqvga);
}


void Ov7670::init() {

	I2C_write(OV7670_I2C_ADDR, REG_COM7, COM7_RESET);
	_delay_ms(200);

    
		init_rgb565_qqvga();
//		init_yuv_qqvga();

	I2C_write(OV7670_I2C_ADDR, REG_COM10, COM10_VS_NEG);
  
  transfer_regvals(ov7670_default);
  transfer_regvals(ov7670_default2);
  

/*
  transfer_regvals(ov7670_default_regs2);
  transfer_regvals(ov7670_fmt_yuv4222);
  I2C_write(OV7670_I2C_ADDR, REG_COM10, COM10_VS_NEG);
*/  
}

