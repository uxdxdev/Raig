#include "base/game_world.h"

GameWorld::GameWorld(int worldSize)
{
	m_iWorldSize = worldSize;
	m_Grid.resize(worldSize, std::vector<int>(worldSize, 0));
}

int GameWorld::GetWorldSize()
{
	return m_iWorldSize;
}

int GameWorld::GetCellX(int x)
{
	return x / CELL_SIZE;
}

int GameWorld::GetCellY(int y)
{
	return y / CELL_SIZE;
}

int GameWorld::GetCellZ(int z)
{
	return z / CELL_SIZE;
}

int GameWorld::GetCellState(int x, int y, int z)
{
	return m_Grid[x][z];
}

void GameWorld::SetCellState(int x, int y, int z, int state)
{
	m_Grid[x][z] = state;
}
