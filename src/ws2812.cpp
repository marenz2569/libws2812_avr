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

#include <stdlib.h>

#include "ws2812.h"

WS2812::WS2812(uint16_t num_leds)
{
	count_led = num_leds;
	
	pixels = (uint8_t*)malloc(count_led*3);
}

WS2812::~WS2812(void)
{
	free(pixels);
}

uint16_t WS2812::get_led_count(void)
{
	return count_led;
}

Color WS2812::get_rgb_at(uint16_t index)
{
	Color pixel;

	if (index < count_led) {
		uint16_t tmp;
		tmp = index * 3;

		pixel.r = pixels[OFFSET_R(tmp)];
		pixel.g = pixels[OFFSET_G(tmp)];
		pixel.b = pixels[OFFSET_B(tmp)];
	}

	return pixel;
}

uint8_t WS2812::set_rgb_at(uint16_t index, Color &pixel)
{
	if (index < count_led) {
		uint16_t tmp;
		tmp = index * 3;

		pixels[OFFSET_R(tmp)] = pixel.r;
		pixels[OFFSET_G(tmp)] = pixel.g;
		pixels[OFFSET_B(tmp)] = pixel.b;

		return 1;
	}
	return 0;
}

void WS2812::sync(void)
{
	*ws2812_port_reg |= pinMask;
	ws2812_sendarray_mask(pixels,3*count_led,pinMask,(uint8_t*) ws2812_port,(uint8_t*) ws2812_port_reg);
}

/* &PORTB, &DDRB, DDB2 */
void WS2812::set_output(const volatile uint8_t* port, volatile uint8_t* reg, uint8_t pin)
{
	pinMask = (1<<pin);
	ws2812_port = port;
	ws2812_port_reg = reg;
}
