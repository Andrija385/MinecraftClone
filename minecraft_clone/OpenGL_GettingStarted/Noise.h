#pragma once
class Noise
{
private:
	int seed;
public:
	virtual float getNoise(float x, float y) const = 0;
	void setSeed(int seed) { this->seed = seed; }
};

