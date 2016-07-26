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
#include "ws2812_avr_macros.h"

FadeLinear::FadeLinear(Color &cFrom, Color &cTo, uint16_t stepCount, color_fade_dir_t fadeDir) : step_count(stepCount), cCur(cFrom), cur_step(0)
{
	float h_step_;
	h_step_ = cTo.h - cFrom.h;

	switch (fadeDir) {
		case shortest:
			if (fabs(h_step_) > 180.0)
				h_step_ = _change_pos_neg(h_step_) * _360_min_fabs(h_step_);
			break;
		case longest:
			if (fabs(h_step_) < 180.0)
				h_step_ = _change_pos_neg(h_step_) * _360_min_fabs(h_step_);
			break;
		case clockwise:
			if (h_step_ < 0.0)
				h_step_ = _360_min_fabs(h_step_);
			break;
		case counter_clockwise:
			if (h_step_ > 0.0)
				h_step_ = -1.0 * _360_min_fabs(h_step_);
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
	if (cur_step++ < step_count) {
		cCur.hsi2rgb(cCur.h + h_step, cCur.s + s_step, cCur.i + i_step);
		return 1;
	}
	return 0;
}

Color FadeLinear::get_cur_color(void)
{
	return cCur;
}
