#pragma once
#include "Chunk.h"
#include <map>
#include <glm/glm.hpp>
class World
{
private:
	
	std::map<std::pair<int, int>, Chunk*> chunks;
	Noise* noise;

public:
	static constexpr int WWIDTH = 16;
	static constexpr int WLENGTH = 16;
	static constexpr int RENDER_DISTANCE = 12;
	World(Noise* noise) : noise(noise) {
		for (int i = 0; i < WWIDTH; ++i) {
			for (int j = 0; j < WLENGTH; ++j) {
				chunks[std::make_pair(i, j)] = new Chunk(i, j, noise);
			}
		}
	}

	std::vector<Vertex> generateMesh() const;
	void updateChunks(glm::vec2 chunkCoordinates);

	static glm::vec2 toChunkCoordinates(glm::vec3 Pos) {
		int chunkX = (int)std::floor(Pos.x / (Chunk::CWIDTH * Block::BLOCK_SIZE));
		int chunkZ = (int)std::floor(Pos.z / (Chunk::CLENGTH * Block::BLOCK_SIZE));
		return glm::vec2(chunkX, chunkZ);
	}
};

