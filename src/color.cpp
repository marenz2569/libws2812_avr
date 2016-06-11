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
#include "interval.h"

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

FadeLinear::FadeLinear(Color &cFrom, Color &cTo, uint16_t stepCount, color_fade_dir_t fadeDir)
{
	step_count = stepCount;
	cur_step = 0;
	cCur = cFrom;

	float h_step_;
	h_step_ = cTo.h - cFrom.h;

#define _360_min_fabs (360.0 - fabs(h_step_))
#define _change_pos_neg (-1.0 * h_step_ / fabs(h_step_))

	switch (fadeDir) {
		case shortest:
			if (fabs(h_step_) > 180.0)
				h_step_ = _change_pos_neg * _360_min_fabs;
			break;
		case longest:
			if (fabs(h_step_) < 180.0)
				h_step_ = _change_pos_neg * _360_min_fabs;
			break;
		case clockwise:
			if (h_step_ < 0.0)
				h_step_ = _360_min_fabs;
			break;
		case counter_clockwise:
			if (h_step_ > 0.0)
				h_step_ = -1.0 * _360_min_fabs;
			break;
	}
	h_step = h_step_ / (float)step_count;
	s_step = (cTo.s - cFrom.s) / (float)step_count;
	i_step = (cTo.i - cFrom.i) / (float)step_count;
}

FadeLinear::FadeLinear(Color &cFrom, Color &cTo, uint16_t stepCount) : FadeLinear(cFrom, cTo, stepCount, shortest)
{

}

uint8_t FadeLinear::next(void)
{
	if (cur_step++ <= step_count) {
		cCur.hsi2rgb(cCur.h + h_step, cCur.s + s_step, cCur.i + i_step);
		return 1;
	}
	return 0;
}

Color FadeLinear::get_cur_color(void)
{
	return cCur;
}
