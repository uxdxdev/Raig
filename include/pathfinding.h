#ifndef PATHFINDING_CELL_H
#define PATHFINDING_CELL_H

#include "search_cell.h"
#include "game_world.h"
#include <vector>
#include <string>
#include "vector3.h"

class Pathfinding{

public:
	Pathfinding(int worldSize);
	virtual ~Pathfinding();


	void FindPath(std::shared_ptr<Vector3> currentPos, std::shared_ptr<Vector3> targetPos);

	// Reset the path found
	void ResetPath();

	// Print the path stored in the pathToGoal vector
	void PrintPath()
	{
		printf("-----PATH Start-----\n");

		for(int i = 0; i < m_vPathToGoal.size(); i++)
		{
			printf("(%d, %d)\n", m_vPathToGoal[i]->m_iX, m_vPathToGoal[i]->m_iZ);
		}

		printf("-----PATH End-----\n");

	}

	enum State{
		IDLE,
		PROCESSING,
		REQUEST_COMPLETE
	};

	// Get pathfinder state
	State GetState(){ return m_eState; }

	// Returns a pointer to the m_vPathToGoal vector
	std::vector<std::shared_ptr<Vector3> > *GetPathToGoal();

private:
	void ClearOpenList();
	void ClearClosedList();
	void ClearPathToGoal();
	void CleanUp();
	void SetStartAndGoal(SearchCell start, SearchCell goal);
	void PathOpened(int x, int z, float newCost, SearchCell *parent);

	void PrintStatus(std::string message)
	{
		printf("--Pathfinder Status--\n");
		printf("State: %d\nOpen List: %d\nClosed List: %d\nPath list: %d\n", m_eState, (int)m_vOpenList.size(), (int)m_vClosedList.size(), (int)m_vPathToGoal.size());
		printf("Goal Cell: %d\n", ((m_GoalCell) ? m_GoalCell->m_iId : -1));
		printf("Message: %s\n\n", message.c_str());
	}

	// Search world and find target position
	// Set G and H values
	void ContinuePath();

	SearchCell *GetNextCell();

	std::vector<SearchCell*> m_vOpenList;
	std::vector<SearchCell*> m_vClosedList;

	std::vector<std::shared_ptr<Vector3> > m_vPathToGoal;

	SearchCell *m_GoalCell;

	std::unique_ptr<GameWorld> m_pGameWorld;

	State m_eState;

	bool m_bInitializedStartGoal;
};

#endif
