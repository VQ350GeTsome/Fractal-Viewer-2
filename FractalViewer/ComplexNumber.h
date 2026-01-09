#pragma once

#include "Point.h"

#include <cmath>

class ComplexNumber {

public:

	double re, im;

	ComplexNumber(double real = 0.0, double imag = 0.0);

	// Operator overloads
	ComplexNumber inline operator+(const ComplexNumber& other) const {
		return ComplexNumber(re + other.re, im + other.im);
	}
	ComplexNumber inline operator-(const ComplexNumber& other) const {
		return ComplexNumber(re - other.re, im - other.im);
	}
	ComplexNumber inline operator*(const ComplexNumber& other) const {
		return ComplexNumber(re * other.re - im * other.im, re * other.im + im * other.re);
	}
	ComplexNumber inline operator/(const ComplexNumber& other) const {
		double denom = other.re * other.re + other.im * other.im;
		return ComplexNumber((re * other.re + im * other.im) / denom,
							 (im * other.re - re * other.im) / denom);
	}

	// Polar coordinate conversions
	Point toPolar() const;
	ComplexNumber fromPolar(double r, double theta) const;

	// Other methods
	double inline magnitude() const { return std::sqrt(re * re + im * im); }
	double inline magnitudeSquared() const { return re*re + im*im; }

	ComplexNumber inline conjugate() const { return ComplexNumber(re, -im); }
	ComplexNumber pow(double exponent) const;
};

