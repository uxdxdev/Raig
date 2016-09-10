#include "base/game_world.h"
#include <iostream>

namespace base{

GameWorld::GameWorld(int worldSize)
{
	m_iWorldSize = worldSize;
	m_Grid.resize(worldSize, std::vector<int>(worldSize, 0));
}

GameWorld::GameWorld(int width, int height)
{
    printf("ctor GameWorld() Width:%d Height:%d\n", width, height);

    m_iWorldWidth = width;
    m_iWorldHeight = height;
    m_iWorldSize = m_iWorldWidth * m_iWorldHeight;
    m_Grid.resize(width, std::vector<int>(height, 0));
}

int GameWorld::GetWorldWidth()
{
    return m_iWorldWidth;
}

int GameWorld::GetWorldHeight()
{
    return m_iWorldHeight;
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

}
