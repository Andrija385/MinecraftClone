#pragma once
class Noise
{
protected:
	int seed;
public:
	Noise(int seed) :seed(seed) {}
	virtual float getNoise(float x, float y) const = 0;
	virtual void setSeed(int seed) { this->seed = seed; }
};

