#include "Block.h"

inline TextureCoords Block::getUV(BlockFace face) const
{
    if (blockType == BlockType::STONE) {
        return { 22 * TEXTURE_TILE_SIZE, 3 * TEXTURE_TILE_SIZE };
    }
    if (blockType == BlockType::GRASS) {
        switch (face) {
        case BlockFace::FRONT:case BlockFace::BACK:case BlockFace::LEFT:case BlockFace::RIGHT:
            return { 28 * TEXTURE_TILE_SIZE, 24 * TEXTURE_TILE_SIZE };
        case BlockFace::UP:
            return { 29 * TEXTURE_TILE_SIZE, 22 * TEXTURE_TILE_SIZE };
        case BlockFace::DOWN:
            return { 24*TEXTURE_TILE_SIZE,29*TEXTURE_TILE_SIZE };
        }
    }
    if (blockType == BlockType::DIRT) {
        return { 24 * TEXTURE_TILE_SIZE,29 * TEXTURE_TILE_SIZE };
    }
    if (blockType == BlockType::OAK_LOG) {
        switch (face) {
        case BlockFace::FRONT:case BlockFace::BACK:case BlockFace::LEFT:case BlockFace::RIGHT:
            return { 17 * TEXTURE_TILE_SIZE, 3 * TEXTURE_TILE_SIZE };
        case BlockFace::UP:case BlockFace::DOWN:
            return { 18 * TEXTURE_TILE_SIZE,3 * TEXTURE_TILE_SIZE };
        }
    }
    if (blockType == BlockType::OAK_LEAVES) {
        return { 4 * TEXTURE_TILE_SIZE,24 * TEXTURE_TILE_SIZE };
    }
    return { 0,0 };
}

//return coordinates given back lower left xyz
std::vector<Vertex> Block::generateFaceMesh(BlockFace face, float x, float y, float z) const
{
    if (blockType == BlockType::AIR)
        return std::vector<Vertex>();
    TextureCoords tc = getUV(face);
    switch (face) {
    case BlockFace::FRONT:
        return std::vector<Vertex>{
            { x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y, z + BLOCK_SIZE, tc.u, tc.v},
        };
    case BlockFace::BACK:
        return std::vector<Vertex>{
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x, y + BLOCK_SIZE, z, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE },
            { x, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x + BLOCK_SIZE, y, z, tc.u, tc.v },
        };
    case BlockFace::UP:
        return std::vector<Vertex>{
            { x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v },
        };
    case BlockFace::DOWN:
        return std::vector<Vertex>{
            { x, y, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y, z, tc.u, tc.v },
        };
    case BlockFace::LEFT:
        return std::vector<Vertex>{
            { x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE },
            { x, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x, y, z, tc.u, tc.v },
        };
    case BlockFace::RIGHT:
        return std::vector<Vertex>{
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE },
            { x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v },
            { x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u, tc.v },
        };
    }

    return std::vector<Vertex>();
}

void Block::appendFaceMesh(std::vector<Vertex>& mesh, BlockFace face, float x, float y, float z) const
{
    if (blockType == BlockType::AIR)
        return;
    TextureCoords tc = getUV(face);
    switch (face) {
    case BlockFace::FRONT:
        mesh.push_back({ x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y, z + BLOCK_SIZE, tc.u, tc.v });
        return;
    case BlockFace::BACK:
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x, y + BLOCK_SIZE, z, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x + BLOCK_SIZE, y, z, tc.u, tc.v });
        return;
    case BlockFace::UP:
        mesh.push_back({ x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v });
        return;
    case BlockFace::DOWN:
        mesh.push_back({ x, y, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y, z, tc.u, tc.v });
        return;
    case BlockFace::LEFT:
        mesh.push_back({ x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y + BLOCK_SIZE, z, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x, y, z + BLOCK_SIZE, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x, y, z, tc.u, tc.v });
        return;
    case BlockFace::RIGHT:
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z, tc.u + TEXTURE_TILE_SIZE, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, tc.u, tc.v + TEXTURE_TILE_SIZE });
        mesh.push_back({ x + BLOCK_SIZE, y, z, tc.u + TEXTURE_TILE_SIZE, tc.v });
        mesh.push_back({ x + BLOCK_SIZE, y, z + BLOCK_SIZE, tc.u, tc.v });
        return;
    }
    return;
}
