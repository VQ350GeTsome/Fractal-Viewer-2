#include "Fractal.h"

COLORREF Fractal::computePixel(int x, int y) {

	ComplexNumber z, c;

	if (halfW > x) {
		z = ComplexNumber(0.0, 0.0);
		c = getComplexFromXY(x, y);
	} else {
		z = getComplexFromXY(x, y);
		c = juliaC;
	}

	int escape = iterate(z, c);

	// Map escape value to a gradient later.
	int color = escape * 5;

	return COLORREF(RGB(color % 256, color % 256, color % 256));
}

int Fractal::iterate(ComplexNumber z, ComplexNumber c) {
	int iterations = 0;
	while (4 >= z.magnitudeSquared() && maxIterations > iterations) {
		z = fractalFunction(z, c);
		iterations++;
	}
	return iterations;
}

ComplexNumber Fractal::getComplexFromXY(int x, int y) {

	bool leftSide = (x < halfW);
	
	double nx = leftSide ? (double) x / halfW : (double) (x - halfW) / halfW;

	double ny = (double)y / h;

	ComplexNumber center = leftSide ? centerMandel : centerJulia;

	double viewWidth = 3.0 / zoom;
	double viewHeight = viewWidth * aspectRatio * 2;

	double realMin = center.real() - viewWidth / 2;
	double realMax = center.real() + viewWidth / 2;
	double imagMin = center.imag() - viewHeight / 2;
	double imagMax = center.imag() + viewHeight / 2;

	double real = realMin + nx * (realMax - realMin);
	double imag = imagMax - ny * (imagMax - imagMin);

	return ComplexNumber(real, imag);
}


Point Fractal::getXYFromComplex(ComplexNumber c, bool mandelSide) {

	double viewWidth = 3.0 / zoom;
	double viewHeight = viewWidth * aspectRatio;

	ComplexNumber center = mandelSide ? centerMandel : centerJulia;

	double realMin = center.real() - viewWidth / 2;
	double realMax = center.real() + viewWidth / 2;
	double imagMin = center.imag() - viewHeight / 2;
	double imagMax = center.imag() + viewHeight / 2;

	double x = ((c.real() - realMin) / (realMax - realMin)) * w;
	double y = ((imagMax - c.imag()) / (imagMax - imagMin)) * h;

	return Point((int) x, (int) y);
}