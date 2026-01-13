#pragma once

#include <string>

#include <functional>	// For lambda function
#include <windows.h>	// For COLORREF
#include <cstdlib> 
#include <ctime>

#include "ComplexNumber.h"
#include "Point.h"
#include "Gradient.h"

template<typename T>
using fractalFunc = std::function<ComplexNumber<T>(ComplexNumber<T>, ComplexNumber<T>)>;

template<typename T>
class Fractal {
	
public:

	Fractal(int width, int height, fractalFunc<T> func) : fractalFunction(func) {

		// Seed random
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		ComplexNumber<T> goodJulias[] = {
			ComplexNumber<T>(-0.4, 0.6),
			ComplexNumber<T>(0.285, 0.0),
			ComplexNumber<T>(-0.70176, -0.3842),
			ComplexNumber<T>(-0.835, -0.2321),
			ComplexNumber<T>(-0.8, 0.156),
			ComplexNumber<T>(0.355, 0.355)
		};

		int idx = std::rand() % 6;
		juliaC = goodJulias[idx];

		setNewDimensions(width, height);
		gradient.setInsideColor(insideColor, maxIterations);
	}

	int computePixel(int x, int y) {

		ComplexNumber<T> z, c;

		if (halfW > x) {
			z = ComplexNumber<T>(0.0, 0.0);
			c = getComplexFromXY(x, y);
		}
		else {
			z = getComplexFromXY(x, y);
			c = juliaC;
		}

		return iterate(z, c);
	}

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
	inline void setNewCenter(int x, int y) {
		if (x < halfW) centerMandel = getComplexFromXY(x, y);
		else centerJulia = getComplexFromXY(x, y);
	}
	inline void setNewJuliaC(int x, int y) { if (halfW >= x) juliaC = getComplexFromXY(x, y); }
	inline void setNewGradient(Gradient g) { gradient = g; }
	inline void setNewFractal(fractalFunc<T> func) { fractalFunction = func; }

	inline void zoomInOut(int x, int y, double delta) {
		// If no zoom return
		if (delta == 1) return;
		if (0 > delta) return;

		bool mandel = (halfW > x);

		// Complex of mouse before zoom
		ComplexNumber<T> before = getComplexFromXY(x, y);

		// Apply zoom
		if (mandel) zoomM *= delta; else zoomJ *= delta;

		// Complex of mouse after zoom
		ComplexNumber<T> after = getComplexFromXY(x, y);

		// Adjust center to keep point under mouse the same
		if (mandel) centerMandel += (before - after);
		else centerJulia += (before - after);
		
	}
	inline void changeGradientSize(int delta) {
		gradient.changeGradientSize(delta);
	}

	inline void pan(int ox, int oy, int nx, int ny) {
		ComplexNumber<T> before = getComplexFromXY(ox, oy),
			after = getComplexFromXY(nx, ny),
			delta = before - after;

		if (halfW > ox) centerMandel += delta;
		else centerJulia += delta;
	}

	inline void reset() {
		centerMandel = ComplexNumber<T>(-0.6, 0.0);
		centerJulia = ComplexNumber<T>();
		zoomM = 4.0;
		zoomJ = 3.5;
	}

	std::string toString(int x, int y) {
		ComplexNumber<T> mouse = getComplexFromXY(x, y);

		std::string side = (halfW > x) ? "Mandel" : "Julia";

		return std::string
		(
			"Side: " + side + "\n" +
			"Complex: " + mouse.toString() + "\n" +
			"Zoom: " + std::to_string((halfW > x) ? (1.0 / zoomM) : (1.0 / zoomJ)) + "\n"
			"Width: " + std::to_string((halfW > x) ? zoomM : zoomJ) + "\n"
		);
	}

	inline uint32_t applyGradient(int iterations) { return gradient.getColor(iterations); }

	ComplexNumber<T> getComplexFromXY(int x, int y) {

		bool leftSide = (x < halfW);

		double nx = leftSide ? (double)x / halfW : (double)(x - halfW) / halfW;

		double ny = (double)y / h;

		ComplexNumber<T> center = leftSide ? centerMandel : centerJulia;
		double zoom = leftSide ? zoomM : zoomJ;

		double viewWidth = zoom;
		double viewHeight = viewWidth * aspectRatio * 2;

		double realMin = center.re - viewWidth / 2;
		double realMax = center.re + viewWidth / 2;
		double imagMin = center.im - viewHeight / 2;
		double imagMax = center.im + viewHeight / 2;

		double real = realMin + nx * (realMax - realMin);
		double imag = imagMax - ny * (imagMax - imagMin);

		return ComplexNumber<T>(real, imag);
	}
	Point getXYFromComplex(ComplexNumber<T> c, bool mandelSide) {

		ComplexNumber<T> center = mandelSide ? centerMandel : centerJulia;
		double zoom = mandelSide ? zoomM : zoomJ;

		double viewWidth = zoom;
		double viewHeight = viewWidth * aspectRatio * 2;

		double realMin = center.re - viewWidth / 2;
		double realMax = center.re + viewWidth / 2;
		double imagMin = center.im - viewHeight / 2;
		double imagMax = center.im + viewHeight / 2;

		double nx = (c.re - realMin) / (realMax - realMin);
		double ny = (imagMax - c.im) / (imagMax - imagMin);

		int x = mandelSide ? (int)(nx * halfW) : (int)(nx * halfW + halfW),
			y = (int)(ny * h);

		return Point(x, y);
	}

private:

	int w, h, halfW;
	double aspectRatio;

	int maxIterations = 256;
	long double zoomM = 4.0, zoomJ = 4.0;

	uint32_t insideColor = 0x000000;

	Gradient gradient = Gradient();

	ComplexNumber<T> juliaC = NULL;
	
	ComplexNumber<T> centerMandel = ComplexNumber<T>(), centerJulia = ComplexNumber<T>();

	fractalFunc<T> fractalFunction;

	int iterate(ComplexNumber<T> z, ComplexNumber<T> c) {
		int iterations = 0;
		while (4 >= z.magnitudeSquared() && maxIterations > iterations) {
			z = fractalFunction(z, c);
			iterations++;
		}
		return iterations;
	}

};

