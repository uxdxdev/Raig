#ifndef PATHFINDING_CELL_H
#define PATHFINDING_CELL_H

#include "vector3.h"
#include "search_cell.h"
#include "game_world.h"
#include <vector>

class Pathfinding{
public:
	Pathfinding(int worldSize);
	virtual ~Pathfinding(void);

	void FindPath(Vector3 currentPos, Vector3 targetPos);
	Vector3 NextPathPos();

	// Reset the path found
	void ResetPath()
	{
		m_eState = IDLE;
		m_bInitializedStartGoal = false;
	}

	// Print the path stored in the pathToGoal vector
	void PrintPath()
	{
		printf("--Pathfinder Status--\n");
		printf("State: %d\n", m_eState);
		printf("-----PATH Start-----\n");

		for(int i = 0; i < m_vPathToGoal.size(); i++)
		{
			printf("(%d, %d)\n", m_vPathToGoal[i]->m_iX, m_vPathToGoal[i]->m_iZ);
		}

		printf("-----PATH End-----\n");

	}

	void ClearOpenList() { m_vOpenList.clear(); }
	void ClearVisitedList() { m_vVisitedList.clear(); }
	void ClearPathToGoal() { m_vPathToGoal.clear(); }
	bool m_bInitializedStartGoal;

	enum State{
		IDLE,
		PROCESSING,
		REQUEST_COMPLETE
	};

	// Get pathfinder state
	State GetState(){ return m_eState; }
	std::vector<Vector3*> GetPathToGoal()
	{
		m_eState = IDLE;
		return m_vPathToGoal;
	}

private:
	void SetStartAndGoal(SearchCell start, SearchCell goal);
	void PathOpened(int x, int z, float newCost, SearchCell *parent);
	SearchCell *GetNextCell();

	// Search world and find target position
	// Set G and H values
	void ContinuePath();

	SearchCell *m_StartCell;
	SearchCell *m_GoalCell;

	std::vector<SearchCell*> m_vOpenList;
	std::vector<SearchCell*> m_vVisitedList;
	std::vector<Vector3*> m_vPathToGoal;

	std::unique_ptr<GameWorld> m_pGameWorld;

	State m_eState;
};

#endif
