/*

The MIT License (MIT)

Copyright (c) 2016 David Morton

https://github.com/damorton/raig.git

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef _INCLUDE_GAME_WORLD_H_
#define _INCLUDE_GAME_WORLD_H_

#include "search_cell.h"
#include <vector>

#define CELL_SIZE 1

class GameWorld{
public:
	GameWorld(int worldSize)
	{
		//printf("ctor GameWorld()\n");

		m_iWorldSize = worldSize;
		m_Grid.resize(worldSize, std::vector<int>(worldSize, 0));
	}

	virtual ~GameWorld()
	{
		//printf("dtor ~GameWorld()\n");
	}

	int GetWorldSize(){ return m_iWorldSize; }
	int GetCellX(int x){ return x / CELL_SIZE; }
	int GetCellZ(int z){ return z / CELL_SIZE; }

	enum CellState{
		CELL_BLOCKED = 5,
		CELL_OPEN
	};

	// Grid is 2D for now
	int GetCellState(int x, int y, int z){ return m_Grid[x][z]; }
	void SetCellState(int x, int y, int z, CellState state)
	{
		printf("Setting cell X:%d Z:%d to state %d\n", x, z, state);
		m_Grid[x][z] = state;
	}

private:

	int m_iWorldSize;
	std::vector< std::vector<int> > m_Grid;
};
#endif
