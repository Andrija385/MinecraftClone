#pragma once
#include "Noise.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>
class PerlinNoise2D :public Noise
{
private:
	std::vector<int>p;

	float fade(float t) const {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	float lerp(float a, float b, float t) const {
		return a + t * (b - a);
	}

	float grad(int hash, float x, float y) const {
		switch (hash & 3) {
		case 0: return  x + y;
		case 1: return -x + y;
		case 2: return  x - y;
		case 3: return -x - y;
		default: return 0; // unreachable
		}
	}
public:
	PerlinNoise2D(int seed) :Noise(seed) {
		setSeed(seed);
	}
	void setSeed(int seed) override {
		Noise::setSeed(seed);
		p.resize(256);
		std::iota(p.begin(), p.end(), 0);
		std::default_random_engine engine(seed);
		std::shuffle(p.begin(), p.end(), engine);
		p.insert(p.end(), p.begin(), p.end()); // duplicate for overflow
	}

	float getNoise(float x, float y) const override {
		x /= 16.0f;
		y /= 16.0f;
		int X = static_cast<int>(std::floor(x)) & 255;
		int Y = static_cast<int>(std::floor(y)) & 255;

		float xf = x - std::floor(x);
		float yf = y - std::floor(y);

		float u = fade(xf);
		float v = fade(yf);

		int aa = p[p[X] + Y];
		int ab = p[p[X] + Y + 1];
		int ba = p[p[X + 1] + Y];
		int bb = p[p[X + 1] + Y + 1];

		float x1 = lerp(grad(aa, xf, yf), grad(ba, xf - 1, yf), u);
		float x2 = lerp(grad(ab, xf, yf - 1), grad(bb, xf - 1, yf - 1), u);
		float result = lerp(x1, x2, v);

		return (result + 1.0f) / 2.0f * 16.0f; // Normalize to [0, 1]
	}
};

