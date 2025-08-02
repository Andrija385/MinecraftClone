#pragma once
#include "Noise.h"
#include <cmath>
class SimpleNoise : public Noise
{
public:
	SimpleNoise(int seed) :Noise(seed) {}
	float getNoise(float x, float y) const override {
		return sin(x / 50) * sin(y / 50) * 20 + sin(x / 20) * sin(y / 20) * 8 + 10 +sin(x / 7) * sin(y / 7) * 3;
	}
};

