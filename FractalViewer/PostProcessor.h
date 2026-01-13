#pragma once

#include <stdint.h> 

using color = uint32_t;

static class PostProcessor {
public:

	bool sum = false;

	color* process(color* image, int width, int height) {
		if (sum) image = summation(image, width, height);
	}

private:

	color* summation(color* image, int width, int height) {
		for (int x = 0; width > x; x++) for (int y = 0; height > y; y++) {

		}
	}

};
