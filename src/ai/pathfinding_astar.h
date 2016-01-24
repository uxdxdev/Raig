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
#ifndef _INCLUDE_PATHFINDING_ASTAR_H
#define _INCLUDE_PATHFINDING_ASTAR_H

#include <memory>
#include <vector>
#include <string>

#include "base/game_world.h"
#include "base/vector3.h"

class SearchCell;

class AStar{

public:
	enum State{
		IDLE,
		PROCESSING,
		REQUEST_COMPLETE
	};

	AStar(int worldSize);

	~AStar();

	void FindPath(std::shared_ptr<base::Vector3> currentPos, std::shared_ptr<base::Vector3> targetPos);

	// Reset the path found
	void ResetPath();

	// Print the path stored in the pathToGoal vector
	void PrintPath();

	// Get pathfinder state
	State GetState();

	void SetState(State state);

	// Returns a pointer to the m_vPathToGoal vector
	std::vector<std::shared_ptr<base::Vector3> > *GetPathToGoal();

	void Update();

	// Pathfinding retains ownership of GameWorld object
	GameWorld *GetGameWorld();

private:

	void ClearOpenList();

	void ClearClosedList();

	void ClearPathToGoal();

	void CleanUp();

	void SetStartAndGoal(SearchCell start, SearchCell goal);

	void ProcessCell(int x, int z, float newCost, SearchCell *parent);

	void PrintStatus(std::string message);

	SearchCell *GetNextCell();

	State m_eState;

	std::vector<SearchCell*> m_vOpenList;

	std::vector<SearchCell*> m_vClosedList;

	SearchCell *m_GoalCell;

	std::vector<std::shared_ptr<base::Vector3> > m_vPathToGoal;

	std::unique_ptr<GameWorld> m_pGameWorld;

	bool m_bInitializedStartGoal;
};

#endif
