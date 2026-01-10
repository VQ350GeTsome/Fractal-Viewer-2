#pragma once

#include "Point.h"

#include <cmath>

class ComplexNumber {

public:

	// Real and imaginary parts
	double re, im;

	ComplexNumber(double real = 0.0, double imag = 0.0);

	// Operator overloads

	// Addition
	inline ComplexNumber operator+(const ComplexNumber& other) const {
		return ComplexNumber(re + other.re, im + other.im);
	}
	// Addition assignment
	inline ComplexNumber operator+=(const ComplexNumber& other) {
		re += other.re; im += other.im;
		return *this;
	}
	// Subtraction
	inline ComplexNumber operator-(const ComplexNumber& other) const {
		return ComplexNumber(re - other.re, im - other.im);
	}
	// Subtraction assignment
	inline ComplexNumber operator-=(const ComplexNumber& other) {
		re -= other.re; im -= other.im;
		return *this;
	}
	// Multiplication
	inline ComplexNumber operator*(const ComplexNumber& other) const {
		return ComplexNumber(re * other.re - im * other.im, re * other.im + im * other.re);
	}
	// Multiplication assignment
	inline ComplexNumber operator*=(const ComplexNumber& other) {
		double real = re * other.re - im * other.im;
		im = re * other.im + im * other.re;
		re = real;
		return *this;
	}
	// Division
	inline ComplexNumber operator/(const ComplexNumber& other) const {
		double denom = other.re * other.re + other.im * other.im;
		return ComplexNumber((re * other.re + im * other.im) / denom,
							 (im * other.re - re * other.im) / denom);
	}
	// Division assignment
	inline ComplexNumber operator/=(const ComplexNumber& other) {
		double denom = other.re * other.re + other.im * other.im;
		double real = (re * other.re + im * other.im) / denom;
		im = (im * other.re - re * other.im) / denom;
		re = real;
		return *this;
	}
	// Exponentiation ( De Moivre's theorem )
	inline ComplexNumber operator^(double exponent) const { return pow(exponent); }

	// Polar coordinate conversions

	// Converts this complex number to polar coordinates (r, theta)
	Point toPolar() const;
	// Returns a new complex number from polar coordinates (r, theta)
	ComplexNumber fromPolar(double r, double theta) const;

	// Other methods

	// Calculates and returns the magnitude of the complex number
	inline double magnitude() const { return std::sqrt(re * re + im * im); }
	// Calculates and returns the squared magnitude of the complex number
	inline double magnitudeSquared() const { return re*re + im*im; }
	// Returns a new complex number that is a blend between this one and another based on parameter t (0 <= t <= 1)
	inline ComplexNumber blend(const ComplexNumber& other, double t) const {
		return ComplexNumber(re * (1 - t) + other.re * t, im * (1 - t) + other.im * t);
	}

	// Exponentiation

	// Returns a new complex number that is this one raised to the given exponent using De Moivre's theorem
	ComplexNumber pow(double exponent) const;
	// Returns a new complex number that is this one squared
	inline ComplexNumber square() const { return ComplexNumber(re * re - im * im, 2 * re * im); }

	// Transformations

	// Returns a new complex number that is this one scaled by a factor
	inline ComplexNumber scale(double factor) const { return ComplexNumber(re * factor, im * factor); }
	// Returns a new complex number that is this one scaled by the inverse of a factor
	inline ComplexNumber scaleInverse(double factor) const { return ComplexNumber(re / factor, im / factor); }
	// Returns a new complex number that is the negation of this one
	inline ComplexNumber negate() const { return ComplexNumber(-re, -im); }
	// Returns a new complex number that is the conjugate of this one
	inline ComplexNumber conjugate() const { return ComplexNumber(re, -im); }
	// Returns a new complex number that is the reciprocal of this one
	inline ComplexNumber reciprocal() const {
		double denom = re * re + im * im;
		return ComplexNumber(re / denom, -im / denom);
	}
	
};

