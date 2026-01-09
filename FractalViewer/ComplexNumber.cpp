#include "ComplexNumber.h"
#include <cmath>

ComplexNumber::ComplexNumber(double real, double imag) : re(real), im(imag) {}

// Component Getters
double ComplexNumber::real() const { return re; }
double ComplexNumber::imag() const { return im; }

// Operator overloads
ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
	return ComplexNumber(re + other.re, im + other.im);
}
ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
	return ComplexNumber(re - other.re, im - other.im);
}
ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) const {
	return ComplexNumber(re * other.re - im * other.im, re * other.im + im * other.re);
}
ComplexNumber ComplexNumber::operator/(const ComplexNumber& other) const {
	double denom = other.re * other.re + other.im * other.im;
	return ComplexNumber((re * other.re + im * other.im) / denom,
		(im * other.re - re * other.im) / denom);
}

// Polar coordinate conversions
std::pair<double, double> ComplexNumber::toPolar() const {
	double r = magnitude();
	double theta = std::atan2(im, re);
	return std::make_pair(r, theta);
}
ComplexNumber ComplexNumber::fromPolar(double r, double theta) const {
	return ComplexNumber(r * std::cos(theta), r * std::sin(theta));
}

// Other methods
double ComplexNumber::magnitude() const { 
	return std::sqrt(this->magnitudeSquared());
}
double ComplexNumber::magnitudeSquared() const {
	return re * re + im * im;
}
ComplexNumber ComplexNumber::conjugate() const {
	return ComplexNumber(re, -im);
}
ComplexNumber ComplexNumber::pow(double exponent) const {
	auto polar = toPolar();
	double r = std::pow(polar.first, exponent);
	double theta = polar.second * exponent;
	return fromPolar(r, theta);
}