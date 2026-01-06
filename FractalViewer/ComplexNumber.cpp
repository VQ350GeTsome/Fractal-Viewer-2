#include "ComplexNumber.h"
#include <cmath>

ComplexNumber::ComplexNumber(double real, double imag) : re(real), im(imag) {}

// Component Getters
double inline ComplexNumber::real() const { return re; }
double inline ComplexNumber::imag() const { return im; }

// Operator overloads
ComplexNumber inline ComplexNumber::operator+(const ComplexNumber& other) const {
	return ComplexNumber(re + other.re, im + other.im);
}
ComplexNumber inline ComplexNumber::operator-(const ComplexNumber& other) const {
	return ComplexNumber(re - other.re, im - other.im);
}
ComplexNumber inline ComplexNumber::operator*(const ComplexNumber& other) const {
	return ComplexNumber(re * other.re - im * other.im, re * other.im + im * other.re);
}
ComplexNumber inline ComplexNumber::operator/(const ComplexNumber& other) const {
	double denom = other.re * other.re + other.im * other.im;
	return ComplexNumber((re * other.re + im * other.im) / denom,
		(im * other.re - re * other.im) / denom);
}

// Polar coordinate conversions
std::pair<double, double> inline ComplexNumber::toPolar() const {
	double r = magnitude();
	double theta = std::atan2(im, re);
	return std::make_pair(r, theta);
}
ComplexNumber inline ComplexNumber::fromPolar(double r, double theta) const {
	return ComplexNumber(r * std::cos(theta), r * std::sin(theta));
}

// Other methods
double inline ComplexNumber::magnitude() const { 
	return std::sqrt(this->magnitudeSquared());
}
double inline ComplexNumber::magnitudeSquared() const {
	return re * re + im * im;
}
ComplexNumber inline ComplexNumber::conjugate() const {
	return ComplexNumber(re, -im);
}
ComplexNumber inline ComplexNumber::pow(double exponent) const {
	auto polar = toPolar();
	double r = std::pow(polar.first, exponent);
	double theta = polar.second * exponent;
	return fromPolar(r, theta);
}