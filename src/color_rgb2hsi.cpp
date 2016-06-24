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

#include "color.h"

/* derived form http://www.cse.usf.edu/~mshreve/rgb-to-hsi */
void Color::rgb2hsi(uint8_t r_, uint8_t g_, uint8_t b_)
{
	r = r_;
	g = g_;
	b = b_;

	float h_, s_, i_;
	i_ = (float)(r_ + g_ + b_) / 3.0 / 255.0;

	/* normalize rgb values */
	float sum = r_ + g_ + b_;
	float n_r = (float) r_ / sum;
	float n_g = (float) g_ / sum;
	float n_b = (float) b_ / sum;

	h_ = acos(0.5 * ((n_r - n_g) + (n_r - n_b)) / sqrt(pow((n_r - n_g), 2.0) + ((n_r - n_b) * (n_g - n_b))));

	if (n_b > n_g)
		h_ = 2 * M_PI - h_;

	s_ = 1 - 3 * fmin(n_r, fmin(n_g, n_b));

	h = h_ * 180.0 / M_PI;
	s = s_;
	i = i_;
}
