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
	ComplexNumber inline operator+=(const ComplexNumber& other) {
		re += other.re; im += other.im;
		return *this;
	}
	ComplexNumber inline operator-(const ComplexNumber& other) const {
		return ComplexNumber(re - other.re, im - other.im);
	}
	ComplexNumber inline operator-=(const ComplexNumber& other) {
		re -= other.re; im -= other.im;
		return *this;
	}
	ComplexNumber inline operator*(const ComplexNumber& other) const {
		return ComplexNumber(re * other.re - im * other.im, re * other.im + im * other.re);
	}
	ComplexNumber inline operator*=(const ComplexNumber& other) {
		double real = re * other.re - im * other.im;
		im = re * other.im + im * other.re;
		re = real;
		return *this;
	}
	ComplexNumber inline operator/(const ComplexNumber& other) const {
		double denom = other.re * other.re + other.im * other.im;
		return ComplexNumber((re * other.re + im * other.im) / denom,
							 (im * other.re - re * other.im) / denom);
	}
	ComplexNumber inline operator/=(const ComplexNumber& other) {
		double denom = other.re * other.re + other.im * other.im;
		double real = (re * other.re + im * other.im) / denom;
		im = (im * other.re - re * other.im) / denom;
		re = real;
		return *this;
	}

	// Polar coordinate conversions
	Point toPolar() const;
	ComplexNumber fromPolar(double r, double theta) const;

	// Other methods
	double inline magnitude() const { return std::sqrt(re * re + im * im); }
	double inline magnitudeSquared() const { return re*re + im*im; }

	ComplexNumber inline scale(double factor) const { return ComplexNumber(re * factor, im * factor); }
	ComplexNumber inline scaleInverse(double factor) const { return ComplexNumber(re / factor, im / factor); }

	ComplexNumber inline blend(const ComplexNumber& other, double t) const {
		return ComplexNumber(re * (1 - t) + other.re * t, im * (1 - t) + other.im * t);
	}

	ComplexNumber inline reciprocal() const { 
		double denom = re * re + im * im; 
		return ComplexNumber(re / denom, -im / denom); 
	}

	ComplexNumber pow(double exponent) const;
	ComplexNumber inline square() const { return ComplexNumber(re * re - im * im, 2 * re * im); }

	ComplexNumber inline negate() const { return ComplexNumber(-re, -im); }
	ComplexNumber inline conjugate() const { return ComplexNumber(re, -im); }
	
};

