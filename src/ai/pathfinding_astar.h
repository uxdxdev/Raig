// Copyright (c) 2016 David Morton
// Use of this source code is governed by a license that can be
// found in the LICENSE file.

#ifndef AI_PATHFINDING_ASTAR_H
#define AI_PATHFINDING_ASTAR_H

#include <memory>
#include <vector>
#include <string>

#include "base/game_world.h"
#include "base/vector3.h"
#include "base/search_cell.h"

namespace ai {

class AStar{

public:
	enum State{
		IDLE,
		PROCESSING,
		REQUEST_COMPLETE
	};

	AStar(int worldSize);

    AStar(int width, int height);

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

	// Pathfinding retains ownership of base::GameWorld object
	base::GameWorld *GetGameWorld();

private:

	void ClearOpenList();

	void ClearClosedList();

	void ClearPathToGoal();

	void CleanUp();

	void SetStartAndGoal(base::SearchCell start, base::SearchCell goal);

	void ProcessCell(int x, int z, float newCost, base::SearchCell *parent);

	void PrintStatus(std::string message);

	base::SearchCell *GetNextCell();

	State m_eState;

	std::vector<base::SearchCell*> m_vOpenList;

	std::vector<base::SearchCell*> m_vClosedList;

	base::SearchCell *m_GoalCell;

	std::vector<std::shared_ptr<base::Vector3> > m_vPathToGoal;

	std::unique_ptr<base::GameWorld> m_pGameWorld;

	bool m_bInitializedStartGoal;
};

}

#endif
