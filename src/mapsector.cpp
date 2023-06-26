/*
Minetest
Copyright (C) 2013 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "mapsector.h"
#include "exceptions.h"
#include "mapblock.h"
#include "serialization.h"

MapSector::MapSector(Map *parent, v2s16 pos, IGameDef *gamedef):
		m_parent(parent),
		m_pos(pos),
		m_gamedef(gamedef)
{
}

MapSector::~MapSector()
{
	deleteBlocks();
}

void MapSector::deleteBlocks()
{
	// Clear cache
	m_block_cache = nullptr;

	// Delete all blocks
	m_blocks.clear();
}

MapBlock *MapSector::getBlockBuffered(s16 y)
{
	MapBlock *block;

	if (m_block_cache && y == m_block_cache_y) {
		return m_block_cache;
	}

	// If block doesn't exist, return NULL
	auto it = m_blocks.find(y);
	block = it != m_blocks.end() ? it->second.get() : nullptr;

	// Cache the last result
	m_block_cache_y = y;
	m_block_cache = block;

	return block;
}

MapBlock *MapSector::getBlockNoCreateNoEx(s16 y)
{
	return getBlockBuffered(y);
}

std::unique_ptr<MapBlock> MapSector::createBlankBlockNoInsert(s16 y)
{
	assert(getBlockBuffered(y) == nullptr); // Pre-condition

	v3s16 blockpos_map(m_pos.X, y, m_pos.Y);

	return std::make_unique<MapBlock>(m_parent, blockpos_map, m_gamedef);
}

MapBlock *MapSector::createBlankBlock(s16 y)
{
	std::unique_ptr<MapBlock> block_u = createBlankBlockNoInsert(y);
	MapBlock *block = block_u.get();

	m_blocks[y] = std::move(block_u);

	return block;
}

void MapSector::insertBlock(std::unique_ptr<MapBlock> block)
{
	s16 block_y = block->getPos().Y;

	MapBlock *block2 = getBlockBuffered(block_y);
	if (block2) {
		throw AlreadyExistsException("Block already exists");
	}

	v2s16 p2d(block->getPos().X, block->getPos().Z);
	assert(p2d == m_pos);

	// Insert into container
	m_blocks[block_y] = std::move(block);
}

void MapSector::deleteBlock(MapBlock *block)
{
	detachBlock(block);
	// returned smart-ptr is dropped
}

std::unique_ptr<MapBlock> MapSector::detachBlock(MapBlock *block)
{
	s16 block_y = block->getPos().Y;

	// Clear from cache
	m_block_cache = nullptr;

	// Remove from container
	auto it = m_blocks.find(block_y);
	assert(it != m_blocks.end());
	std::unique_ptr<MapBlock> ret = std::move(it->second);
	assert(ret.get() == block);
	m_blocks.erase(it);

	// Mark as removed
	block->makeOrphan();

	return ret;
}

void MapSector::getBlocks(MapBlockVect &dest)
{
	dest.reserve(dest.size() + m_blocks.size());
	for (auto &block : m_blocks) {
		dest.push_back(block.second.get());
	}
}
