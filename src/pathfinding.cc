#include "include/pathfinding.h"

Pathfinding::Pathfinding(GameWorld *gameWorld)
{
	m_GoalCell = NULL;
	m_StartCell = NULL;
	m_pGameWorld = gameWorld;
	m_bInitializedStartGoal = false;
	m_bFoundGoal = false;
}

Pathfinding::~Pathfinding(void)
{

}

void Pathfinding::FindPath(Vector3 currentPos, Vector3 targetPos)
{
	if(!m_bInitializedStartGoal)
	{
		// set to unsigned int
		for(int i = 0; i < m_vOpenList.size(); i++)
		{
			delete m_vOpenList[i];
		}
		m_vOpenList.clear();

		for(int i = 0; i < m_vVisitedList.size(); i++)
		{
			delete m_vVisitedList[i];
		}
		m_vVisitedList.clear();

		for(int i = 0; i < m_vPathToGoal.size(); i++)
		{
			delete m_vPathToGoal[i];
		}
		m_vPathToGoal.clear();

		// Initialize start
		SearchCell start;
		start.m_iCoordinateX = m_pGameWorld->GetCellX(currentPos.m_iX);
		start.m_iCoordinateZ = m_pGameWorld->GetCellZ(currentPos.m_iZ);

		// Initialize goal
		SearchCell goal;
		goal.m_iCoordinateX = m_pGameWorld->GetCellX(targetPos.m_iX);
		goal.m_iCoordinateZ = m_pGameWorld->GetCellZ(targetPos.m_iZ);

		SetStartAndGoal(start, goal);
		m_bInitializedStartGoal = true;
	}

	if(m_bInitializedStartGoal)
	{
		ContinuePath();
	}
}

void Pathfinding::SetStartAndGoal(SearchCell start, SearchCell goal)
{
	m_StartCell = new SearchCell(start.m_iCoordinateX, start.m_iCoordinateZ, NULL, m_pGameWorld->GetWorldSize());
	m_GoalCell = new SearchCell(goal.m_iCoordinateX, goal.m_iCoordinateZ, &goal, m_pGameWorld->GetWorldSize());

	m_StartCell->G = 0;
	m_StartCell->H = m_StartCell->ManhattanDistance(m_GoalCell);
	m_StartCell->m_pParent = 0;

	m_vOpenList.push_back(m_StartCell);
}

SearchCell *Pathfinding::GetNextCell()
{
	float bestF = 99999.0f;
	int cellIndex = -1;
	SearchCell *nextCell = NULL;

	for(int i = 0; i < m_vOpenList.size(); i++)
	{
		if(m_vOpenList[i]->GetF() < bestF)
		{
			bestF = m_vOpenList[i]->GetF();
			cellIndex = i;
		}
	}

	if(cellIndex >= 0)
	{
		nextCell = m_vOpenList[cellIndex];
		m_vVisitedList.push_back(nextCell);
		m_vOpenList.erase(m_vOpenList.begin() + cellIndex);
	}

	return nextCell;
}

void Pathfinding::PathOpened(int x, int z, float newCost, SearchCell *parent)
{
	// Walls etc.

	if(m_pGameWorld->GetCellState(x, z) == CELL_BLOCKED)
	{
		return;
	}


	int id = z * m_pGameWorld->GetWorldSize() + x;

	for(int i = 0; i < m_vVisitedList.size(); i++)
	{
		if(id == m_vVisitedList[i]->m_iId)
		{
			return;
		}
	}

	SearchCell *newChild = new SearchCell(x, z, parent, m_pGameWorld->GetWorldSize());
	newChild->G = newCost;
	newChild->H = parent->ManhattanDistance(m_GoalCell);

	for(int i = 0; i < m_vOpenList.size(); i++)
	{
		if(id == m_vOpenList[i]->m_iId)
		{
			float newF = newChild->G + newCost + m_vOpenList[i]->H;

			// if newF is less than one in the list replace it
			if(m_vOpenList[i]->GetF() > newF)
			{
				m_vOpenList[i]->G = newChild->G + newCost;
				m_vOpenList[i]->m_pParent = newChild;
			}
			else // if the newF is not better
			{
				delete newChild;
				return;
			}
		}
	}

	m_vOpenList.push_back(newChild);
}

void Pathfinding::ContinuePath()
{
	for(int x = 0; x < 4; x++)
	{
		if(m_vOpenList.empty())
		{
			return;
		}

		SearchCell *currentCell = GetNextCell();

		// If we have reached the goal cell
		if(currentCell->m_iId == m_GoalCell->m_iId)
		{
			m_GoalCell->m_pParent = currentCell->m_pParent;

			SearchCell *getPath;

			for(getPath = m_GoalCell; getPath != NULL; getPath = getPath->m_pParent)
			{
				m_vPathToGoal.push_back(new Vector3(getPath->m_iCoordinateX * CELL_SIZE, 0, getPath->m_iCoordinateZ * CELL_SIZE));
			}

			m_bFoundGoal = true;
			return;
		}
		else
		{
			// right side
			PathOpened(currentCell->m_iCoordinateX + 1, currentCell->m_iCoordinateZ, currentCell->G + 1, currentCell);

			// left side
			PathOpened(currentCell->m_iCoordinateX - 1, currentCell->m_iCoordinateZ, currentCell->G + 1, currentCell);

			// top cell
			PathOpened(currentCell->m_iCoordinateX, currentCell->m_iCoordinateZ + 1, currentCell->G + 1, currentCell);

			// bottom cell
			PathOpened(currentCell->m_iCoordinateX, currentCell->m_iCoordinateZ - 1, currentCell->G + 1, currentCell);

			// topleft diagonal
			PathOpened(currentCell->m_iCoordinateX - 1, currentCell->m_iCoordinateZ + 1, currentCell->G + 1.414f, currentCell);

			// topright diagonal
			PathOpened(currentCell->m_iCoordinateX + 1, currentCell->m_iCoordinateZ + 1, currentCell->G + 1.414f, currentCell);

			// bottom left diagonal
			PathOpened(currentCell->m_iCoordinateX - 1, currentCell->m_iCoordinateZ - 1, currentCell->G + 1.414f, currentCell);

			// bottom right
			PathOpened(currentCell->m_iCoordinateX + 1, currentCell->m_iCoordinateZ - 1, currentCell->G + 1.414f, currentCell);

			for(int i = 0; i < m_vOpenList.size(); i++)
			{
				if(currentCell->m_iId == m_vOpenList[i]->m_iId)
				{
					m_vOpenList.erase(m_vOpenList.begin() + i);
				}
			}
		}
	}
}


// Not used
Vector3 Pathfinding::NextPathPos()
{
	int index = 1;
	Vector3 nextPosition;
	nextPosition.m_iX = m_vPathToGoal[m_vPathToGoal.size() - index]->m_iX + (CELL_SIZE/2);
	nextPosition.m_iZ = m_vPathToGoal[m_vPathToGoal.size() - index]->m_iZ + (CELL_SIZE/2);

	if(index < m_vPathToGoal.size())
	{
		//if((nextCell - object->Position()).Length() < object->GetRadius())
		{
			//m_vPathToGoal.erase(m_vPathToGoal.end() - index);
		}
	}

	return nextPosition;
}

