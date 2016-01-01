#include "../include/pathfinding.h"

Pathfinding::Pathfinding(int worldSize)
{
	printf("ctor Pathfinding()\n");

	m_GoalCell = NULL;
	m_StartCell = NULL;
	m_pGameWorld = std::unique_ptr<GameWorld> (new GameWorld(worldSize));
	m_bInitializedStartGoal = false;
	m_eState = IDLE;
}

Pathfinding::~Pathfinding()
{
	printf("dtor ~Pathfinding()\n");
	CleanUp();
}

void Pathfinding::CleanUp()
{
	// Delete all objects pointed to by the stored pointers in the vector.
	// Clear each vector for every new request
	for(unsigned int i = 0; i < m_vOpenList.size(); i++)
	{
		delete m_vOpenList[i];
	}
	m_vOpenList.clear();

	for(unsigned int i = 0; i < m_vVisitedList.size(); i++)
	{
		delete m_vVisitedList[i];
	}
	m_vVisitedList.clear();

	for(unsigned int i = 0; i < m_vPathToGoal.size(); i++)
	{
		delete m_vPathToGoal[i];
	}
	m_vPathToGoal.clear();

	// m_GoalCell memory needs to be released on each path request otherwise
	// a leak would be created each time a new goal cell in created. The m_StartCell
	// is stored in the m_vOpenList and is deleted when the vector is cleaned
	delete m_GoalCell;
}

void Pathfinding::FindPath(Vector3 currentPos, Vector3 targetPos)
{
	if(!m_bInitializedStartGoal)
	{
		//printf("Initializing Start Goal X:%d Z:%d\n", currentPos.m_iX, currentPos.m_iZ);
		CleanUp();

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
		m_eState = PROCESSING;
	}

	if(m_bInitializedStartGoal)
	{
		ContinuePath();
	}
}

void Pathfinding::SetStartAndGoal(SearchCell start, SearchCell goal)
{
	//printf("Called SetStartAndGoal()\n");
	m_StartCell = new SearchCell(start.m_iCoordinateX, start.m_iCoordinateZ, NULL, m_pGameWorld->GetWorldSize());
	m_GoalCell = new SearchCell(goal.m_iCoordinateX, goal.m_iCoordinateZ, &goal, m_pGameWorld->GetWorldSize());

	m_StartCell->G = 0;
	m_StartCell->H = m_StartCell->ManhattanDistance(m_GoalCell);
	m_StartCell->m_pParent = 0;

	//printf("StartCell X:%d Z:%d GoalCell X:%d Z:%d\n", m_StartCell->m_iCoordinateX, m_StartCell->m_iCoordinateZ, m_GoalCell->m_iCoordinateX, m_GoalCell->m_iCoordinateZ);
	//printf("StartCell G:%f H:%f\n", m_StartCell->G, m_StartCell->H);

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

	//printf("Called GetNextCell() : X:%d Z:%d\n", nextCell->m_iCoordinateX, nextCell->m_iCoordinateZ);

	return nextCell;
}

void Pathfinding::PathOpened(int x, int z, float newCost, SearchCell *parent)
{
	//printf("Called PathOpened() : X%d Z:%d\n", x, z);

	if(x < 0 || x > m_pGameWorld->GetWorldSize() - 1 || z < 0 || z > m_pGameWorld->GetWorldSize() - 1)
	{
		// Out of bounds
		return;
	}

	// Walls etc.
	if(m_pGameWorld->GetCellState(x, z) == GameWorld::CELL_BLOCKED)
	{
		printf("CELL_BLOCKED X:%d Z:%d", x, z);
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

	//printf("New child ID %d X%d Z:%d G:%f H:%f\n",newChild->m_iId, newChild->m_iCoordinateX, newChild->m_iCoordinateZ, newChild->G, newChild->H);

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
	//printf("Called ContinuePath()\n");

	if(m_vOpenList.empty())
	{
		return;
	}

	SearchCell *currentCell = GetNextCell();

	// If we have reached the goal cell
	if(currentCell->m_iId == m_GoalCell->m_iId)
	{
		//printf("Goal cell reached\n");

		m_GoalCell->m_pParent = currentCell->m_pParent;

		SearchCell *getPath;

		for(getPath = m_GoalCell; getPath != NULL; getPath = getPath->m_pParent)
		{
			m_vPathToGoal.push_back(new Vector3(getPath->m_iCoordinateX * CELL_SIZE, 0, getPath->m_iCoordinateZ * CELL_SIZE));
		}

		m_eState = REQUEST_COMPLETE;
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

std::vector<Vector3*> *Pathfinding::GetPathToGoal()
{
	// Once the path to goal has been accessed the pathfinder
	// can be reset to IDLE in order to process more requests
	m_eState = IDLE;
	return &m_vPathToGoal;
}

void Pathfinding::ResetPath()
{
	m_eState = IDLE;
	m_bInitializedStartGoal = false;
}


