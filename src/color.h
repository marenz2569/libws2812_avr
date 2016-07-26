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

#ifndef COLOR_H__
#define COLOR_H__

#include <inttypes.h>

/**
 * A class used to save and convert hsi and rgb values
 */
class Color
{
	/**
	 * check if the rgb values of 2 Color objects are the same
	 * @param a first Color object
	 * @param b second Color object
	 */
	friend bool operator ==(Color a, Color b)
	{
		if (a.r != b.r)
			return false;
		if (a.g != b.g)
			return false;
		if (a.b != b.b)
			return false;
		return true;
	}
public:
	/**
	 * red value
	 */
	uint8_t r;
	/**
	 * green value
	 */
	uint8_t g;
	/**
	 * blue value
	 */
	uint8_t b;
	/**
	 * hue value
	 */
	float h;
	/**
	 * saturation value
	 */
	float s;
	/**
	 * intensity value
	 */
	float i;

	/**
	 * convert hsi to rgb
	 * @param h_ hue value
	 * @param s_ saturation value
	 * @param i_ intensity value
	 */
	void hsi2rgb(float h_, float s_, float i_);
	/**
	 * convert rgb to hsi
	 * @param r_ red value
	 * @param g_ green value
	 * @param b_ blue value
	 */
	void rgb2hsi(uint8_t r_, uint8_t g_, uint8_t b_);
};

/**
 * A type to define the fadeing direction
 */
enum color_fade_dir_t
{
	shortest,
	longest,
	clockwise,
	counter_clockwise
};

/**
 * A class used to fade linear between two points in hsi color space
 */
class FadeLinear
{
private:
	uint16_t step_count;

	uint16_t cur_step;

	float h_step, s_step, i_step;

	Color cCur;

public:
	/**
	 * @param cFrom Color object as fade origin
	 * @param cTo Color oject as fade result
	 * @param stepCount number of fadeing steps
	 * @param fadeDir fade direction in hsi color space
	 */
	FadeLinear(Color &cFrom, Color &cTo, uint16_t stepCount, color_fade_dir_t fadeDir);
	/**
	 * calls constructor {@link #FadeLinear(Color &cFrom, Color &cTo, uint16_t stepCount, color_fade_dir_t fadeDir)}
	 * with fadeDir = shortest
	 */
	FadeLinear(Color &cFrom, Color &cTo, uint16_t stepCount);

	/**
	 * updates Color object in this class, that saves current fade color
	 * @return 1 if successful, 0 if not
	 */
	uint8_t next(void);

	/**
	 * @return Color object
	 */
	Color get_cur_color(void);

};

/**
 * A class is used to fade from the color to a grayscale with i = deltaI / 2 to the second color
 */
class FadeThroughIAxis {
private:
	uint16_t step_count;

	uint16_t cur_step;

	float from_step_count, to_step_count;

	float h_from_step, s_from_step, i_from_step;

	float h_to_step, s_to_step, i_to_step;

	Color cCur;
public:
	FadeThroughIAxis(Color& cFrom, Color& cTo, uint16_t stepCount);

	uint8_t next(void);

	Color get_cur_color(void);
};

#endif
