#ifndef PATHFINDING_CELL_H
#define PATHFINDING_CELL_H

#include "vector3.h"
#include "search_cell.h"
#include "game_world.h"
#include <vector>

class Pathfinding{
public:
	Pathfinding(GameWorld *gameWorld);
	~Pathfinding(void);

	void FindPath(Vector3 currentPos, Vector3 targetPos);
	Vector3 NextPathPos();
	void ResetPath(){ m_bInitializedStartGoal = false; }
	void PrintPath()
	{
		printf("Called PrintPath()\n");
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
	bool m_bFoundGoal;

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

	GameWorld *m_pGameWorld;
};

#endif
