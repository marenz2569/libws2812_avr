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

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>

#include <ws2812.h>

int main(void)
{
#define LED_C 4
	WS2812 led(LED_C);
	led.set_output(&PORTB, &DDRB, DDB1);

	Color redColor;
	redColor.h = 0.0;
	redColor.s = 1.0;
	redColor.i = 1.0;
	Color cyanColor;
	cyanColor.h = 180.0;
	cyanColor.s = 0.5;
	cyanColor.i = 0.3;
	Color curColor;

#define hue_step 0.00625
#define delay 125
#define fade_1_8 ((uint16_t)(360.0 / hue_step / 4.0))

	FadeLinear r2c(redColor, cyanColor, fade_1_8, clockwise);
	FadeLinear c2r(cyanColor, redColor, fade_1_8, clockwise);
	FadeLinear fade = r2c;

	bool mode = false;
	int j;

	for (;;) {
		/* fade from red to cyan and back*/
		if(mode) {
			fade = r2c; mode=false;}
		else {
			fade = c2r; mode=true;}
		while (fade.next() > 0) {
			curColor = fade.get_cur_color();
			for (j=0;j<LED_C;j++)
				led.set_rgb_at(j, curColor);
			led.sync();
			_delay_us(delay);
		}

		_delay_ms(2000);
	}
	return 0;
}
