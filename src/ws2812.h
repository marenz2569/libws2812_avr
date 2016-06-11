/*
 * A library for avr microcontrollers to control WS2812 leds
 *
 * Copyright (C) 2016 Markus Schmidl
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef WS2812_H__
#define WS2812_H__

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "color.h"

#define OFFSET_R(r) r+1
#define OFFSET_G(g) g	
#define OFFSET_B(b) b+2	

class WS2812
{
public:
	/**
	 * @param num_led number of WS2812 leds
	 */
	WS2812(uint16_t num_led);
	~WS2812(void);

	/**
	 * sets a pin as signal output
	 * @param port PORTx
	 * @param reg DDRx
	 * @param pin DDBy
	 */
	void set_output(const volatile uint8_t* port, volatile uint8_t* reg, uint8_t pin);

	/**
	 * get number of leds
	 * @return number of leds
	 */
	uint16_t get_led_count(void);

	/**
	 * get rgb values of a led
	 * @param index number of the led
	 * @return Color object with rgb values
	 */
	Color get_rgb_at(uint16_t index);
	/**
	 * set rgb values of a led
	 * @param index number of the led
	 * @param pixel Color object with rgb values
	 * @return 1 if successful, 0 if not
	 */
	uint8_t set_rgb_at(uint16_t index, Color &pixel);

	/**
	 * synchronize the leds with the set color values
	 */
	void sync(void);
	
private:
	uint16_t count_led;
	uint8_t *pixels;

	void ws2812_sendarray_mask(uint8_t *array, uint16_t length, uint8_t pinmask, uint8_t *port, uint8_t *portreg);

	const volatile uint8_t *ws2812_port;
	volatile uint8_t *ws2812_port_reg;
	uint8_t pinMask; 
};



#endif
