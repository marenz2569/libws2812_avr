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

#include <avr/io.h>
#include <util/delay.h>

#include <ws2812.h>

#define LED_C	4

int main(void)
{
	WS2812 led(LED_C);
	led.set_output(&PORTB, &DDRB, DDB1);

	Color blank;
	Color blue;

	blank.r = 255;
	blank.g = 0;
	blank.b = 0;

	blue.r = 0;
	blue.g = 0;
	blue.b = 255;

#define delay		1000000

	led.set_rgb_at(0, blue);
	led.set_rgb_at(2, blue);
	led.set_rgb_at(1, blank);
	led.set_rgb_at(3, blank);

	int j;

	for (;;) {
		for (j=0;j<LED_C;j++) {
			/*if (led.get_rgb_at(j).b == 255)*/
			if (led.get_rgb_at(j) == blue)
				led.set_rgb_at(j, blank);
			else
				led.set_rgb_at(j, blue);
		}
		led.sync();

		_delay_us(delay);
	}
	return 0;
}
