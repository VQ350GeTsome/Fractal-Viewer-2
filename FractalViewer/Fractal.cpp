#include "Fractal.h"

int Fractal::computePixel(int x, int y) {

	ComplexNumber z, c;

	if (halfW > x) {
		z = ComplexNumber(0.0, 0.0);
		c = getComplexFromXY(x, y);
	} else {
		z = getComplexFromXY(x, y);
		c = juliaC;
	}

	return iterate(z, c);
}

int Fractal::iterate(ComplexNumber z, ComplexNumber c) {
	int iterations = 0;
	while (4 >= z.magnitudeSquared() && maxIterations > iterations) {
		z = fractalFunction(z, c);
		iterations++;
	}
	return iterations;
}

uint32_t Fractal::applyGradient(int iterations) {
	if (iterations == maxIterations) return 0xFF000000; // Black for points inside the set.
	// Simple gradient from blue to red.
	uint8_t r = (iterations * 9) % 256;
	uint8_t g = 0;
	uint8_t b = (255 - (iterations * 9)) % 256;
	return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

ComplexNumber Fractal::getComplexFromXY(int x, int y) {

	bool leftSide = (x < halfW);
	
	double nx = leftSide ? (double) x / halfW : (double) (x - halfW) / halfW;

	double ny = (double)y / h;

	ComplexNumber center = leftSide ? centerMandel : centerJulia;
	double zoom = leftSide ? zoomM : zoomJ;

	double viewWidth = 3.0 / zoom;
	double viewHeight = viewWidth * aspectRatio * 2;

	double realMin = center.re - viewWidth / 2;
	double realMax = center.re + viewWidth / 2;
	double imagMin = center.im - viewHeight / 2;
	double imagMax = center.im + viewHeight / 2;

	double real = realMin + nx * (realMax - realMin);
	double imag = imagMax - ny * (imagMax - imagMin);

	return ComplexNumber(real, imag);
}
Point Fractal::getXYFromComplex(ComplexNumber c, bool mandelSide) {

	ComplexNumber center = mandelSide ? centerMandel : centerJulia;
	double zoom = mandelSide ? zoomM : zoomJ;

	double viewWidth = 3.0 / zoom;
	double viewHeight = viewWidth * aspectRatio * 2; 

	double realMin = center.re - viewWidth / 2;
	double realMax = center.re + viewWidth / 2;
	double imagMin = center.im - viewHeight / 2;
	double imagMax = center.im + viewHeight / 2;

	double nx = (c.re - realMin) / (realMax - realMin);
	double ny = (imagMax - c.im) / (imagMax - imagMin);

	int x = mandelSide ? (int) (nx * halfW) : (int) (nx * halfW + halfW), 
		y = (int)(ny * h);

	return Point(x, y);
}

