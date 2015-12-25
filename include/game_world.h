#ifndef GAME_WORLD
#define GAME_WORLD

#include "search_cell.h"

#define CELL_SIZE 1

enum CellState{
	CELL_OPEN,
	CELL_BLOCKED
};

class GameWorld{
	GameWorld(int worldSize)
	{
		m_iWorldSize = worldSize;
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
	SearchCell m_Grid[m_iWorldSize][m_iWorldSize];
};
#endif GAME_WORLD
