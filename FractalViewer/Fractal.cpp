#include "Fractal.h"

COLORREF Fractal::computePixel(int x, int y) {

	ComplexNumber z = ComplexNumber(0.0, 0.0);
	ComplexNumber c = getComplexFromXY(x, y);

	int escape = iterate(z, c);

	// Map escape value to a gradient later.
	int color = escape;

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

	double aspectRatio = (double) h / w;

	double viewWidth = 3.0 / zoom; // base width of view, scaled by zoom
	double viewHeight = viewWidth * aspectRatio;

	double realMin = center.real() - viewWidth / 2;
	double realMax = center.real() + viewWidth / 2;
	double imagMin = center.imag() - viewHeight / 2;
	double imagMax = center.imag() + viewHeight / 2;

	double real = realMin + (x / (double) w) * (realMax - realMin);
	double imag = imagMax - (y / (double) h) * (imagMax - imagMin); // flip Y-axis

	return ComplexNumber(real, imag);
}

Point Fractal::getXYFromComplex(ComplexNumber c) {
	double aspectRatio = (double) h / w;

	double viewWidth = 3.0 / zoom;
	double viewHeight = viewWidth * aspectRatio;

	double realMin = center.real() - viewWidth / 2;
	double realMax = center.real() + viewWidth / 2;
	double imagMin = center.imag() - viewHeight / 2;
	double imagMax = center.imag() + viewHeight / 2;

	double x = ((c.real() - realMin) / (realMax - realMin)) * w;
	double y = ((imagMax - c.imag()) / (imagMax - imagMin)) * h;

	return Point((int) x, (int) y);
}