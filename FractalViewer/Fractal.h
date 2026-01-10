#pragma once

#include <functional>	// For lambda function
#include <windows.h>	// For COLORREF
#include <cstdlib> 
#include <ctime>

#include "ComplexNumber.h"
#include "Point.h"
#include "Gradient.h"

class Fractal {

public:

	Fractal(int width, int height, std::function<ComplexNumber(ComplexNumber, ComplexNumber)> func) : fractalFunction(func) {

		// Seed random
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		ComplexNumber goodJulias[] = {
			ComplexNumber(-0.4, 0.6),
			ComplexNumber(0.285, 0.0),
			ComplexNumber(-0.70176, -0.3842),
			ComplexNumber(-0.835, -0.2321),
			ComplexNumber(-0.8, 0.156),
			ComplexNumber(0.355, 0.355)
		};

		int idx = std::rand() % 6;
		juliaC = goodJulias[idx];

		setNewDimensions(width, height);
		gradient.setInsideColor(insideColor, maxIterations);
	}

	int computePixel(int x, int y);

	inline void setNewDimensions(int width, int height) {
		w = width; h = height;
		halfW = w / 2;
		aspectRatio = (double) h / w;
	}

	inline void setNewIterations(int iterations) {
		maxIterations = iterations;
		setInsideColor(insideColor);
	}
	inline void setInsideColor(uint32_t color) {
		insideColor = color;
		gradient.setInsideColor(insideColor, maxIterations);
	}

	inline void zoomInOut(int x, int y, double delta) {
		// If no zoom return
		if (delta == 1) return;
		if (0 > delta) return;

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
	inline void changeGradientSize(int delta) {
		gradient.changeGradientSize(delta);
	}

	inline void setNewCenter(int x, int y) {
		if (x < halfW) centerMandel = getComplexFromXY(x, y);
		else centerJulia = getComplexFromXY(x, y);
	}
	inline void setNewJuliaC(int x, int y) {
		if (halfW >= x) {
			juliaC = getComplexFromXY(x, y);
		}
	}

	inline uint32_t applyGradient(int iterations) { return gradient.getColor(iterations); }

	ComplexNumber getComplexFromXY(int x, int y);
	Point getXYFromComplex(ComplexNumber cl, bool mandelSide);

private:

	int w, h, halfW;
	double aspectRatio;

	int maxIterations = 256;
	double zoomM = 1.0, zoomJ = 1.0;

	uint32_t insideColor = 0x000000;

	Gradient gradient = Gradient();

	ComplexNumber juliaC = NULL;
	
	ComplexNumber centerMandel = ComplexNumber(-0.5, 0.0), centerJulia = ComplexNumber(0.0, 0.0);

	std::function<ComplexNumber(ComplexNumber, ComplexNumber)> fractalFunction;

	int iterate(ComplexNumber z, ComplexNumber c);

};

