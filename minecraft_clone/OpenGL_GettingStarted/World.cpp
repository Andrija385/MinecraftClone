#include "World.h"
#include <stack>
#include <iostream>

std::vector<Vertex> World::generateMesh() const {
	std::vector<Vertex> res;
	
	for (const auto& kv : chunks) {
		kv.second->appendMesh(res);
	}

	return res;
}

void World::updateChunks(glm::vec2 chunkCoordinates) {
	int chunkX = (int)chunkCoordinates.x;
	int chunkZ = (int)chunkCoordinates.y;
	std::stack<std::pair<int, int>> toRemove;
	for (const auto& kv : chunks) {
		if (abs(kv.first.first - chunkX) > RENDER_DISTANCE || abs(kv.first.second - chunkZ) > RENDER_DISTANCE) {
			toRemove.push(kv.first);
		}
	}
	//std::cout << toRemove.size() << std::endl;
	while (!toRemove.empty()) {
		chunks.erase(toRemove.top());
		toRemove.pop();
	}

	for (int dx = -RENDER_DISTANCE; dx <= RENDER_DISTANCE; ++dx) {
		for (int dz = -RENDER_DISTANCE; dz <= RENDER_DISTANCE; ++dz) {
			int cx = chunkX + dx;
			int cz = chunkZ + dz;
			std::pair<int, int> pos(cx, cz);

			if (chunks.find(pos) == chunks.end()) {
				chunks[pos] = new Chunk(cx, cz, this->noise);
			}
		}
	}
}