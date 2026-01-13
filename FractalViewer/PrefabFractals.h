#pragma once

#include <functional>
#include "ComplexNumber.h"

static struct {
public:
	const std::function<ComplexNumber<long double>(ComplexNumber<long double>, ComplexNumber<long double>)>

	CUSTOM = [](auto z, auto c) {
		return z*z + c;
	},

	MANDELBROT = [](auto z, auto c) {
		return z*z + c;
	},

	BURNINGSHIP = [](auto z, auto c) {
		z = z.abs();
		return z*z + c;
	};

} fractals ;
