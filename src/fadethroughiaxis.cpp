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

FadeThroughIAxis::FadeThroughIAxis(Color& cFrom, Color& cTo, uint16_t stepCount) : step_count(stepCount), cur_step(0), cCur(cFrom)
{
	Color center;
	float deltaI, tri_i_s_from;

	deltaI = fabs(cFrom.i - cTo.i);
	tri_i_s_from = sqrt(pow(deltaI, 2) / 4 + pow(cFrom.s, 2));
	from_step_count = tri_i_s_from / (tri_i_s_from + sqrt(pow(deltaI, 2) / 4 + pow(cTo.s, 2))) * step_count;
	to_step_count = step_count - from_step_count;

	center.i = fmin(cFrom.i, cTo.i) + deltaI / 2;
	center.h = 0.0;
	center.s = 0.0;

	float h_from_step_;
	/* shortest */
	h_from_step_ = center.h - cFrom.h;
	if (fabs(h_from_step_) > 180.0)
		h_from_step_ = _change_pos_neg(h_from_step_) * _360_min_fabs(h_from_step_);
	h_from_step = h_from_step_ / (float)from_step_count;
	s_from_step = (center.s - cFrom.s) / (float)from_step_count;
	i_from_step = (center.i - cFrom.i) / (float)from_step_count;

	float h_to_step_;
	/* shortest */
	h_to_step_ = cTo.h - center.h;
	if (fabs(h_to_step_) > 180.0)
		h_to_step_ = _change_pos_neg(h_to_step_) * _360_min_fabs(h_to_step_);
	h_to_step = h_to_step_;
	s_to_step = (cTo.s - center.s) / (float)to_step_count;
	i_to_step = (cTo.i - center.i) / (float)to_step_count;
}

uint8_t FadeThroughIAxis::next(void)
{
	if (cur_step++ < from_step_count) {
		cCur.hsi2rgb(cCur.h + h_from_step, cCur.s + s_from_step, cCur.i + i_from_step);
		return 1;
	} else if (cur_step++ < step_count) {
		cCur.hsi2rgb(cCur.h + h_to_step, cCur.s + s_to_step, cCur.i + i_to_step);
		return 1;
	}
	return 0;
}

Color FadeThroughIAxis::get_cur_color(void)
{
	return cCur;
}
