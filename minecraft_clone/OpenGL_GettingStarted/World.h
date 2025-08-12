#pragma once
#include "Chunk.h"
#include "Camera.h"
#include <map>
#include <glm/glm.hpp>
#include <iostream>

struct RaycastInfo {
	bool hit;
	BlockType blockType;
	glm::vec3 position;
	glm::vec3 prevPosition;
	float distance;
};

class World
{
private:
	
	std::map<std::pair<int, int>, Chunk*> chunks;
	Noise* noise;
	int seed;

public:
	static constexpr int WWIDTH = 16;
	static constexpr int WLENGTH = 16;
	static constexpr int RENDER_DISTANCE = 12;
	World(int seed) : seed(seed), noise(new PerlinNoise2D(seed)) {
		for (int i = 0; i < WWIDTH; ++i) {
			for (int j = 0; j < WLENGTH; ++j) {
				chunks[std::make_pair(i, j)] = new Chunk(i, j, noise);
			}
		}
	}

	std::vector<Vertex> generateMesh() const;
	void updateChunks(glm::vec2 chunkCoordinates);

	static glm::ivec2 toChunkCoordinates(glm::vec3 Pos) {
		int chunkX = (int)std::floor(Pos.x / (Chunk::CWIDTH * Block::BLOCK_SIZE));
		int chunkZ = (int)std::floor(Pos.z / (Chunk::CLENGTH * Block::BLOCK_SIZE));
		return glm::ivec2(chunkX, chunkZ);
	}
	static glm::ivec3 toBlockCoordinates(glm::vec3 Pos) {
		int x = (int)std::floor(Pos.x / (Block::BLOCK_SIZE));
		int y = (int)std::floor(Pos.y / (Block::BLOCK_SIZE));
		int z = (int)std::floor(Pos.z / (Block::BLOCK_SIZE));
		return glm::ivec3(x, y, z);
	}
	static glm::ivec3 toBlockLocalCoordinates(glm::vec3 Pos) {
		glm::ivec2 chunkPos = toChunkCoordinates(Pos);
		glm::ivec3 blockPos = toBlockCoordinates(Pos);
		glm::ivec3 blockPosLocal = blockPos - glm::ivec3(chunkPos.x * Chunk::CWIDTH, 0, chunkPos.y * Chunk::CLENGTH);
		return blockPosLocal;
	}
	static glm::ivec3 toBlockLocalCoordinates(glm::vec3 Pos, glm::ivec2 chunkPos) {
		glm::ivec3 blockPos = toBlockCoordinates(Pos);
		glm::ivec3 blockPosLocal = blockPos - glm::ivec3(chunkPos.x * Chunk::CWIDTH, 0, chunkPos.y * Chunk::CLENGTH);
		return blockPosLocal;
	}


	RaycastInfo Raycast(glm::vec3 position, glm::vec3 direction, float distance);

	BlockType getBlockType(glm::vec3 position);

	void processLeftMouseButton(const Camera& camera);
	void processRightMouseButton(const Camera& camera);
	bool removeBlockAt(glm::vec3 position);
	bool addBlockAt(glm::vec3 position);
};

