#include "ComplexNumber.h"

#include "Point.h"

#include <cmath>

ComplexNumber::ComplexNumber(double real, double imag) : re(real), im(imag) {}

// Polar coordinate conversions
Point ComplexNumber::toPolar() const {
	double r = magnitude();
	double theta = std::atan2(im, re);
	return Point(r, theta);
}
ComplexNumber ComplexNumber::fromPolar(double r, double theta) const {
	return ComplexNumber(r * std::cos(theta), r * std::sin(theta));
}

// Other methods
ComplexNumber ComplexNumber::pow(double exponent) const {
	Point polar = toPolar();
	double r = std::pow(polar.x, exponent);
	double theta = polar.y * exponent;
	return fromPolar(r, theta);
}