#include "Chunk.h"

inline bool Chunk::isSolid(int i, int j, int k) const
{
	if (i < 0 || j < 0 || k < 0 || i >= CWIDTH || j >= CHEIGHT || k >= CLENGTH) return false;
	return blocks[i][j][k].getBlockType() != BlockType::AIR;
}

void Chunk::generateTerrain()
{
	for (int i = 0; i < CWIDTH; ++i) {
		for (int k = 0; k < CLENGTH; ++k) {
			int cur_height = std::min((int)noise->getNoise(x * CWIDTH * Block::BLOCK_SIZE + i, y * CLENGTH * Block::BLOCK_SIZE + k), CHEIGHT);
			for (int j = 0; j < cur_height; ++j) {
				blocks[i][j][k].setBlockType(BlockType::GRASS);
			}
			if (i % 13 == 7 && k % 17 == 3 && cur_height>0)
				for (int j = cur_height; j < std::min(CHEIGHT, cur_height + 3); ++j)
					blocks[i][j][k].setBlockType(BlockType::OAK_LOG);
		}
	}
}

std::vector<Vertex> Chunk::generateMesh() const
{
	std::vector<Vertex> res;
	res.reserve(CWIDTH * CLENGTH * CHEIGHT * 6 * 6);
	for (int i = 0; i < CWIDTH; ++i) {
		for (int k = 0; k < CLENGTH; ++k) {
			for (int j = 0; j < CHEIGHT; ++j) {
				if (blocks[i][j][k].getBlockType() == BlockType::AIR)continue;

				if(!isSolid(i,j,k+1))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::FRONT, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if(!isSolid(i,j,k-1))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::BACK, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if(!isSolid(i,j+1,k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::UP, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if(!isSolid(i,j-1,k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::DOWN, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if(!isSolid(i+1,j,k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::RIGHT, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if(!isSolid(i-1,j,k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::LEFT, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
			}
		}
	}
	return res;
}

void Chunk::appendMesh(std::vector<Vertex>& res) const
{
	for (int i = 0; i < CWIDTH; ++i) {
		for (int k = 0; k < CLENGTH; ++k) {
			for (int j = 0; j < CHEIGHT; ++j) {
				if (blocks[i][j][k].getBlockType() == BlockType::AIR)continue;

				if (!isSolid(i, j, k + 1))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::FRONT, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if (!isSolid(i, j, k - 1))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::BACK, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if (!isSolid(i, j + 1, k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::UP, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if (!isSolid(i, j - 1, k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::DOWN, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if (!isSolid(i + 1, j, k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::RIGHT, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
				if (!isSolid(i - 1, j, k))
					blocks[i][j][k].appendFaceMesh(res, BlockFace::LEFT, x * CWIDTH * Block::BLOCK_SIZE + i, j, y * CLENGTH * Block::BLOCK_SIZE + k);
			}
		}
	}
}
