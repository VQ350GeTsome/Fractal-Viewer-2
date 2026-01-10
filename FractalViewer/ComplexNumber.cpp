#include "ComplexNumber.h"

#include "Point.h"

#include <cmath>

ComplexNumber::ComplexNumber(double real, double imag) : re(real), im(imag) {}

// Polar coordinate conversions

// Converts this complex number to polar coordinates (r, theta)
Point ComplexNumber::toPolar() const {
	double r = magnitude();
	double theta = std::atan2(im, re);
	return Point(r, theta);
}
// Returns a new complex number from polar coordinates (r, theta)
ComplexNumber ComplexNumber::fromPolar(double r, double theta) const {
	return ComplexNumber(r * std::cos(theta), r * std::sin(theta));
}

// Returns a new complex number that is this one raised to the given exponent using De Moivre's theorem
ComplexNumber ComplexNumber::pow(double exponent) const {
	Point polar = toPolar();
	double r = std::pow(polar.x, exponent);
	double theta = polar.y * exponent;
	return fromPolar(r, theta);
}