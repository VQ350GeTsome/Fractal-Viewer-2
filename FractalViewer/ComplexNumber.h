#pragma once

#include "Point.h"

#include <cmath>
#include <string>

template<typename T>
class ComplexNumber {

public:

	// Real and imaginary parts
	T re, im;

	ComplexNumber(T real = T{}, T imag = T{}) : re(real), im(imag) {}

	// Operator overloads

	// Addition
	inline ComplexNumber<T> operator+(const ComplexNumber<T>& other) const { return ComplexNumber<T>(re + other.re, im + other.im); }
	// Addition assignment
	inline ComplexNumber<T> operator+=(const ComplexNumber<T>& other) {
		re += other.re; im += other.im;
		return *this;
	}
	// Subtraction
	inline ComplexNumber<T> operator-(const ComplexNumber<T>& other) const { return ComplexNumber<T>(re - other.re,
																							im - other.im); }
	// Subtraction assignment
	inline ComplexNumber<T> operator-=(const ComplexNumber<T>& other) {
		re -= other.re; im -= other.im;
		return *this;
	}
	// Multiplication
	inline ComplexNumber<T> operator*(const ComplexNumber<T>& other) const {
		return ComplexNumber<T>(re * other.re - im * other.im,
							 re * other.im + im * other.re);
	}
	// Multiplication assignment
	inline ComplexNumber<T> operator*=(const ComplexNumber<T>& other) {
		double real = re * other.re - im * other.im;
		im = re * other.im + im * other.re;
		re = real;
		return *this;
	}
	// Multiplication with scalar
	inline ComplexNumber<T> operator*(double scalar) const { return ComplexNumber<T>(re * scalar, im * scalar); }
	// Multiplication assignment with scalar
	inline ComplexNumber<T> operator*=(double scalar) {
		re *= scalar;
		im *= scalar;
		return *this;
	}
	// Division
	inline ComplexNumber<T> operator/(const ComplexNumber<T>& other) const {
		double denom = other.re * other.re + other.im * other.im;
		return ComplexNumber<T>((re * other.re + im * other.im) / denom,
							 (im * other.re - re * other.im) / denom);
	}
	// Division assignment
	inline ComplexNumber<T> operator/=(const ComplexNumber<T>& other) {
		double denom = other.re * other.re + other.im * other.im;
		double real = (re * other.re + im * other.im) / denom;
		im = (im * other.re - re * other.im) / denom;
		re = real;
		return *this;
	}
	// Division with scalar
	inline ComplexNumber<T> operator/(double scalar) const { return ComplexNumber<T>(re / scalar, im / scalar); }
	// Division assignment with scalar
	inline ComplexNumber<T> operator/=(double scalar) {
		re /= scalar;
		im /= scalar;
		return *this;
	}
	// Exponentiation ( De Moivre's theorem )
	inline ComplexNumber<T> operator^(double exponent) const { return pow(exponent); }
	// Equality check
	inline bool operator==(const ComplexNumber<T>& other) const { return (re == other.re) && (im == other.im); }
	// Inequality check
	inline bool operator!=(const ComplexNumber<T>& other) const { return (re != other.re) || (im != other.im); }

	// Polar coordinate conversions

	// Converts this complex number to polar coordinates (r, theta)
	inline Point toPolar() const {
		double r = magnitude();
		double theta = std::atan2(im, re);
		return Point(r, theta);
	}
	// Returns a new complex number from polar coordinates (r, theta)
	ComplexNumber<T> fromPolar(double r, double theta) const {
		return ComplexNumber<T>(r * std::cos(theta), r * std::sin(theta));
	}

	// Other methods

	// Calculates and returns the magnitude of the complex number
	inline double magnitude() const { return std::sqrt(re * re + im * im); }
	// Calculates and returns the squared magnitude of the complex number
	inline double magnitudeSquared() const { return re*re + im*im; }
	// Returns a new complex number that is a blend between this one and another based on parameter t (0 <= t <= 1)
	inline ComplexNumber<T> blend(const ComplexNumber<T>& other, double t) const {
		return ComplexNumber<T>(re * (1 - t) + other.re * t,
							 im * (1 - t) + other.im * t);
	}

	// Exponentiation

	// Returns a new complex number that is this one raised to the given exponent using De Moivre's theorem
	ComplexNumber<T> pow(double exponent) const {
		double r = magnitude();
		double theta = std::atan2(im, re);
		double newR = std::pow(r, exponent);
		double newTheta = theta * exponent;
		return ComplexNumber<T>(newR * std::cos(newTheta),
								newR * std::sin(newTheta));
	}

	// Returns a new complex number that is this one squared
	inline ComplexNumber<T> square() const { return ComplexNumber<T>(re * re - im * im, 2 * re * im); }

	// Transformations

	// Returns a new complex number that is this one scaled by a factor
	inline ComplexNumber<T> scale(double factor) const { return ComplexNumber<T>(re * factor, im * factor); }
	// Returns a new complex number that is this one scaled by the inverse of a factor
	inline ComplexNumber<T> scaleInverse(double factor) const { return ComplexNumber<T>(re / factor, im / factor); }
	// Returns a new complex number that is the negation of this one
	inline ComplexNumber<T> negate() const { return ComplexNumber<T>(-re, -im); }
	// Returns a new complex number that is the conjugate of this one
	inline ComplexNumber<T> conjugate() const { return ComplexNumber<T>(re, -im); }
	// Returns a new complex number that is the reciprocal of this one
	inline ComplexNumber<T> reciprocal() const {
		double denom = re * re + im * im;
		return ComplexNumber<T>(re / denom, -im / denom);
	}

	std::string toString() {
		return "(" + std::to_string(roundTo(re, 8)) + ", " + std::to_string(roundTo(im, 8)) + "i)";
	}

private:
	inline double roundTo(float value, int places) {
		double factor = std::pow(10.0f, places);
		return std::round(value * factor) / factor;
	}
	
};