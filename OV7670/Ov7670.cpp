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






void Ov7670::init(char mode) 
{

	I2C_write(OV7670_I2C_ADDR, REG_COM7, COM7_RESET);
	_delay_ms(500);

	I2C_write(OV7670_I2C_ADDR, REG_COM10, COM10_VS_NEG);

  X_Resolution = 0;
  Y_Resolution = 0;

  
  transfer_regvals(ov7670_ORG);
  delay(300); // Setting time for register change
  

    
  switch(mode)
  {
    case MODE_VGA :
    case MODE_QVGA :
      transfer_regvals(ov7670_qvga_ORG); 
  
      X_Resolution = QVGA_WIDTH;
      Y_Resolution = QVGA_HEIGHT;
      break;
      
    case MODE_QQVGA :
    default:
      transfer_regvals(ov7670_qqvga_ORG); 

      X_Resolution = QQVGA_WIDTH;
      Y_Resolution = QQVGA_HEIGHT;
      break;
  }
  delay(300); // Setting time for register change  


}

