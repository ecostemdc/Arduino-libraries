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


#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

class Ov7670 {
public:
	Ov7670();
	void init();
	void nightMode(bool enable);
	void contrast(int8_t);
	void brightness(int8_t);



	uint8_t init_success;


  uint8_t I2C_write(uint8_t slave_address, uint8_t address, uint8_t data); 
  uint8_t I2C_read(uint8_t slave_address, uint8_t address, uint8_t *data); 



	uint8_t transfer_regvals(const struct regval_list *list);
	uint8_t init_rgb444_qqvga();
	uint8_t init_rgb555_qqvga();
	uint8_t init_rgb565_qqvga();
	uint8_t init_yuv_qqvga();

};

#endif /* OV7670_H_ */
