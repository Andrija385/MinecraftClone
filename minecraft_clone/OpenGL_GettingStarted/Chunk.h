#pragma once
#include "Block.h"
#include "SimpleNoise.h"
#include "PerlinNoise2D.h"


class Chunk
{
public:
	static constexpr int CWIDTH = 16;
	static constexpr int CLENGTH = 16;
	static constexpr int CHEIGHT = 512;
private:
	
	Noise* noise;
	int x, y;
	Block blocks[CWIDTH][CHEIGHT][CLENGTH];

	inline bool isSolid(int i, int j, int k) const;
	void generateTerrain();
public:
	Chunk(int x, int y, Noise* noise) :x(x), y(y), noise(noise) {
		generateTerrain();
	}
	std::vector<Vertex> generateMesh() const;
	void appendMesh(std::vector<Vertex>& res) const;
};

