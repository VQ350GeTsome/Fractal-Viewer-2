#pragma once
#include <utility>

class ComplexNumber {

public:

	ComplexNumber(double real = 0.0, double imag = 0.0);

	// Component Getters
	double real() const;
	double imag() const;

	// Operator overloads
	ComplexNumber operator+(const ComplexNumber& other) const;
	ComplexNumber operator-(const ComplexNumber& other) const;
	ComplexNumber operator*(const ComplexNumber& other) const;
	ComplexNumber operator/(const ComplexNumber& other) const;

	// Polar coordinate conversions
	std::pair<double, double> toPolar() const;
	ComplexNumber fromPolar(double r, double theta) const;

	// Other methods
	double magnitude() const;
	double magnitudeSquared() const;
	ComplexNumber conjugate() const;
	ComplexNumber pow(double exponent) const;

private:
	double re, im;
};

