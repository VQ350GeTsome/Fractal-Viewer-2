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

	uint32_t computePixel(int x, int y);

	void setNewDimensions(int width, int height) {
		w = width; h = height;
		halfW = w / 2;
		aspectRatio = (double) h / w;
	}

	void zoomInOut(int x, int y, double delta) { 

		if (delta == 1) return;

		if (x < halfW) {
			//centerMandel = getComplexFromXY(x, y);
			zoomM *= delta;
		} else {
			//centerJulia = getComplexFromXY(x, y);
			zoomJ *= delta;
		}
	}

	void setNewCenter(int x, int y) {
		if (x < halfW) centerMandel = getComplexFromXY(x, y);
		else centerJulia = getComplexFromXY(x, y);
	}

	ComplexNumber getComplexFromXY(int x, int y);
	Point getXYFromComplex(ComplexNumber cl, bool mandelSide);

private:

	int w, h, halfW;
	double aspectRatio;

	int maxIterations = 128;
	double zoomM = 1.0, zoomJ = 1.0;

	ComplexNumber juliaC = ComplexNumber(-0.7, 0.27015);
	
	ComplexNumber centerMandel = ComplexNumber(-0.5, 0.0), centerJulia = ComplexNumber(0.0, 0.0);

	std::function<ComplexNumber(ComplexNumber, ComplexNumber)> fractalFunction;

	int iterate(ComplexNumber z, ComplexNumber c);

};

