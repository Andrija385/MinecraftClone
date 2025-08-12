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

RaycastInfo World::Raycast(glm::vec3 position, glm::vec3 direction, float distance)
{
	float dt = 0.1f;
	glm::vec3 step = dt * direction;
	glm::vec3 current = position;
	glm::vec3 prev = position;
	float t = 0.0f;

	while (t <= distance) {
		BlockType blockType = getBlockType(current);
		if (blockType != BlockType::AIR) {
			return { true, blockType ,current, prev, t };
			//std::cout << (int)blockType << std::endl;
		}
		

		prev = current;
		current = current + step;
		t = t + dt;
	}

	return { false, BlockType::AIR, current-step, current-step, distance};
}

BlockType World::getBlockType(glm::vec3 position) {
	glm::ivec2 chunkPos = toChunkCoordinates(position);
	glm::ivec3 blockPosLocal = toBlockLocalCoordinates(position, chunkPos);
	if (chunks.find({ chunkPos.x, chunkPos.y }) != chunks.end()) {
		//std::cout << "USO\n";
		//std::cout << chunkPos.x << ' ' << chunkPos.y << '\n' << blockPos.x << ' ' << blockPos.y << ' ' << blockPos.z << '\n' << blockPosLocal.x << ' ' << blockPosLocal.y << ' ' << blockPosLocal.z << '\n';
		return chunks[{chunkPos.x, chunkPos.y}]->getBlockType(blockPosLocal);
	}
	return BlockType::AIR;
}

void World::processLeftMouseButton(const Camera& camera){
	RaycastInfo info = this->Raycast(camera.getPosition(), camera.getLookDirection(), 10.0f);
	if (info.blockType != BlockType::AIR) {
		//std::cout << (int)info.blockType << std::endl;
		//std::cout << info.position.x << ' ' << info.position.y << ' ' << info.position.z << std::endl;
		this->removeBlockAt(info.position);
	}
}

void World::processRightMouseButton(const Camera& camera)
{
	RaycastInfo info = this->Raycast(camera.getPosition(), camera.getLookDirection(), 10.0f);
	if (info.blockType != BlockType::AIR) {
		//std::cout << (int)info.blockType << std::endl;
		//std::cout << info.position.x << ' ' << info.position.y << ' ' << info.position.z << std::endl;
		this->addBlockAt(info.prevPosition);
	}
}

bool World::removeBlockAt(glm::vec3 position){
	glm::ivec2 chunkPos = toChunkCoordinates(position);
	glm::ivec3 blockPosLocal = toBlockLocalCoordinates(position, chunkPos);
	if (chunks.find({ chunkPos.x, chunkPos.y }) != chunks.end()) {
		return chunks[{chunkPos.x, chunkPos.y}]->setBlockType(blockPosLocal,BlockType::AIR);
	}
	return false;
}

bool World::addBlockAt(glm::vec3 position)
{
	glm::ivec2 chunkPos = toChunkCoordinates(position);
	glm::ivec3 blockPosLocal = toBlockLocalCoordinates(position, chunkPos);
	if (chunks.find({ chunkPos.x, chunkPos.y }) != chunks.end()) {
		return chunks[{chunkPos.x, chunkPos.y}]->setBlockType(blockPosLocal, BlockType::DIRT);
	}
	return false;
}
