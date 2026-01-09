#pragma once

#include <functional>	// For lambda function
#include <windows.h>	// For COLORREF

#include "ComplexNumber.h"
#include "Point.h"
#include "Gradient.h"

class Fractal {

public:

	Fractal(int width, int height, std::function<ComplexNumber(ComplexNumber, ComplexNumber)> func) : fractalFunction(func) {
		setNewDimensions(width, height);
		gradient.setInsideColor(insideColor, maxIterations);
	}

	int computePixel(int x, int y);

	void setNewDimensions(int width, int height) {
		w = width; h = height;
		halfW = w / 2;
		aspectRatio = (double) h / w;
	}

	void setNewIterations(int iterations) {
		maxIterations = iterations;
		gradient.setInsideColor(insideColor, maxIterations);
	}

	void inline zoomInOut(int x, int y, double delta) {
		// If no zoom return
		if (delta == 1) return;

		bool mandel = (halfW > x);

		// Complex of mouse before zoom
		ComplexNumber before = getComplexFromXY(x, y);

		// Apply zoom
		if (mandel) zoomM *= delta; else zoomJ *= delta;

		// Complex of mouse after zoom
		ComplexNumber after = getComplexFromXY(x, y);

		// Adjust center to keep point under mouse the same
		if (mandel) centerMandel += (before - after);
		else centerJulia += (before - after);
		
	}

	void setNewCenter(int x, int y) {
		if (x < halfW) centerMandel = getComplexFromXY(x, y);
		else centerJulia = getComplexFromXY(x, y);
	}
	void setNewJuliaC(int x, int y) {
		if (halfW >= x) {
			juliaC = getComplexFromXY(x, y);
		}
	}

	uint32_t applyGradient(int iterations) { return gradient.getColor(iterations); }

	ComplexNumber getComplexFromXY(int x, int y);
	Point getXYFromComplex(ComplexNumber cl, bool mandelSide);

private:

	int w, h, halfW;
	double aspectRatio;

	int maxIterations = 256;
	double zoomM = 1.0, zoomJ = 1.0;

	uint32_t insideColor = 0x000000;

	Gradient gradient = Gradient();

	ComplexNumber juliaC = ComplexNumber(-0.85, 0.15);
	
	ComplexNumber centerMandel = ComplexNumber(-0.5, 0.0), centerJulia = ComplexNumber(0.0, 0.0);

	std::function<ComplexNumber(ComplexNumber, ComplexNumber)> fractalFunction;

	int iterate(ComplexNumber z, ComplexNumber c);

};

