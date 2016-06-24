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
#include "macros.h"

/* derived from http://fourier.eng.hmc.edu/e161/lectures/ColorProcessing/node3.html */
void Color::hsi2rgb(float h_, float s_, float i_)
{
	h = h_;
	s = s_;
	i = i_;

	h_ = fmod(h_, 360);
	/* convert h to radiants */
	h_ = M_PI * h_ / 180.0;
	/* clamp s and i to interval [0:1] */
	s_ = clamp_to_0_1(s_);
	i_ = clamp_to_0_1(i_);

	if (s_ == 0.0) {
		r = g = b = i_ * 255;
	} else {
		float r_, g_, b_;
#define hsi2rgb_color1 ((1 - s_) / 3)
#define hsi2rgb_color2 ((1 + s_ * cos(h_) / cos((M_PI / 3.0) - h_)) / 3)
		if ((h_ >= 0.0) && (h_ < 2.0 * M_PI / 3.0)) {
			b_ = hsi2rgb_color1;
			r_ = hsi2rgb_color2;
			g_ = 1 - b_ - r_;
		} else if ((h_ >= 2.0 * M_PI / 3.0) && (h_ < 4.0 * M_PI / 3.0)) {
			h_ -= 2.0 * M_PI / 3.0;
			r_ = hsi2rgb_color1;
			g_ = hsi2rgb_color2;
			b_ = 1 - r_ - g_;
		} else {
			h_ -= 4.0 * M_PI / 3.0;
			g_ = hsi2rgb_color1;
			b_ = hsi2rgb_color2;
			r_ = 1 - g_ - b_;
		}

		r_ = 3 * i_ * r_ * 255;
		g_ = 3 * i_ * g_ * 255;
		b_ = 3 * i_ * b_ * 255;

		/* clamp r, g and b to interval [0:255] */
		r = clamp_to_0_255(r_);
		g = clamp_to_0_255(g_);
		b = clamp_to_0_255(b_);
	}
	
}
