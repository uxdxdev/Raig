#ifndef GAME_WORLD
#define GAME_WORLD

#include "search_cell.h"
#include <vector>

#define CELL_SIZE 1

enum CellState{
	CELL_OPEN,
	CELL_BLOCKED
};

class GameWorld{
public:
	GameWorld(int worldSize)
	{
		printf("ctor GameWorld()\n");

		m_iWorldSize = worldSize;
		m_Grid.resize(worldSize, std::vector<int>(worldSize, 0));
		// initialize grid
	}

	~GameWorld()
	{
		//delete[] m_Grid;
	}

	int GetWorldSize(){ return m_iWorldSize; }
	int GetCellX(int x){ return x / CELL_SIZE; }
	int GetCellZ(int z){ return z / CELL_SIZE; }
	int GetCellState(int x, int z){ return m_Grid[x][z]; }
private:

	int m_iWorldSize;
	std::vector< std::vector<int> > m_Grid;
};
#endif
