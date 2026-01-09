#pragma once

#include <functional>	// For lambda function
#include <windows.h>	// For COLORREF

#include "ComplexNumber.h"
#include "Point.h"

class Fractal {

public:

	Fractal(int width, int height, std::function<ComplexNumber(ComplexNumber, ComplexNumber)> func) : fractalFunction(func) {
		setNewDimensions(width, height);
	}

	COLORREF computePixel(int x, int y);

	void setNewDimensions(int width, int height) {
		w = width; h = height;
		halfW = w / 2;
		aspectRatio = (double) h / w;
	}

	ComplexNumber getComplexFromXY(int x, int y);
	Point getXYFromComplex(ComplexNumber cl, bool mandelSide);

private:

	int w, h, halfW;
	double aspectRatio;

	int maxIterations = 256;
	double zoom = 1.0;

	ComplexNumber juliaC = ComplexNumber(-0.7, 0.27015);
	
	ComplexNumber centerMandel = ComplexNumber(-0.5, 0.0), centerJulia = ComplexNumber(0.0, 0.0);

	std::function<ComplexNumber(ComplexNumber, ComplexNumber)> fractalFunction;

	int iterate(ComplexNumber z, ComplexNumber c);

};

