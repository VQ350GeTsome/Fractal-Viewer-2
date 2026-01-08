#pragma once

#include <functional>	// For lambda function
#include <windows.h>	// For COLORREF

#include "ComplexNumber.h"
#include "Point.h"

class Fractal {

public:

	Fractal(int width, int height, std::function<ComplexNumber(ComplexNumber, ComplexNumber)> func) : w(width), h(height), fractalFunction(func) {}

	COLORREF computePixel(int x, int y);

private:

	int w, h;
	int maxIterations = 256;
	double zoom = 1.0;
	
	ComplexNumber center = ComplexNumber(0.0, -0.5);

	std::function<ComplexNumber(ComplexNumber, ComplexNumber)> fractalFunction;

	int iterate(ComplexNumber z, ComplexNumber c);

	ComplexNumber getComplexFromXY(int x, int y);
	Point getXYFromComplex(ComplexNumber c);


};

