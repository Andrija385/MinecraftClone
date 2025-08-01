#pragma once
#include <vector>


struct TextureCoords {
	float u, v;
};

struct Vertex {
	float x, y, z;
	TextureCoords tc;
};

enum class BlockFace {
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class BlockType {
	GRASS,
	STONE,
	AIR,
	DIRT,
	OAK_LOG,
	OAK_LEAVES
};

class Block
{
public:
	static constexpr float BLOCK_SIZE = 1.0f;
private:
	static constexpr float TEXTURE_TILE_SIZE = 0.03125f;

	BlockType blockType;

	inline TextureCoords getUV(BlockFace face) const;

public:
	Block() :blockType(BlockType::AIR) {}
	Block(BlockType blockType) :blockType(blockType) {}

	void setBlockType(BlockType bt) { this->blockType = bt; }
	BlockType getBlockType() const { return this->blockType; }

	std::vector<Vertex> generateFaceMesh(BlockFace face, float x, float y, float z) const;

	void appendFaceMesh(std::vector<Vertex>& mesh, BlockFace face, float x, float y, float z) const;
};

