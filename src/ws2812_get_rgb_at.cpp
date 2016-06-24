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

#include "ws2812.h"

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
