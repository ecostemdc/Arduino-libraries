/*
 * Ov7670.h
 *
 *  Created on: Aug 26, 2013
 *      Author: arndtjenssen
 */

#ifndef OV7670_H_
#define OV7670_H_

#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>
#include "camera_config.h"



#define MODE_RGB565 2
#define MODE_YUV 		3

#define MODE_VGA 		      0
#define MODE_QVGA 		    1
#define MODE_QQVGA 		    2


#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

class Ov7670 {
public:
	Ov7670();
	void init( char mode );
	void contrast(int8_t);
	void brightness(int8_t);


	uint8_t init_success;
  int X_Resolution;
  int Y_Resolution;

  uint8_t I2C_write(uint8_t slave_address, uint8_t address, uint8_t data); 
  uint8_t I2C_read(uint8_t slave_address, uint8_t address, uint8_t *data); 


	uint8_t transfer_regvals(const struct regval_list *list);

};

#endif /* OV7670_H_ */
