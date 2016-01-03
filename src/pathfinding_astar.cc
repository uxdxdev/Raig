#include "../include/pathfinding_astar.h"

AStar::AStar(int worldSize)
{
	printf("ctor Pathfinding()\n");

	m_GoalCell = NULL;
	m_pGameWorld = std::unique_ptr<GameWorld> (new GameWorld(worldSize));
	m_bInitializedStartGoal = false;
	m_eState = IDLE;
}

AStar::~AStar()
{
	printf("dtor ~Pathfinding()\n");
	CleanUp();
}

void AStar::CleanUp()
{
	PrintStatus("Before CleanUp()");

	// Delete all objects pointed to by the stored pointers in the vector.
	// Clear each vector for every new request
	ClearOpenList();
	ClearClosedList();
	ClearPathToGoal();

	delete m_GoalCell;
	m_GoalCell = NULL;

	PrintStatus("After CleanUp()");
}

void AStar::ClearOpenList()
{
	for(unsigned int i = 0; i < m_vOpenList.size(); i++)
	{
		delete m_vOpenList[i];
	}
	m_vOpenList.clear();
}

void AStar::ClearClosedList()
{
	for(unsigned int i = 0; i < m_vClosedList.size(); i++)
	{
		delete m_vClosedList[i];
	}
	m_vClosedList.clear();
}

void AStar::ClearPathToGoal()
{
	for(unsigned int i = 0; i < m_vPathToGoal.size(); i++)
	{
		//delete m_vPathToGoal[i];
	}
	m_vPathToGoal.clear();
}

void AStar::FindPath(std::shared_ptr<Vector3> currentPos, std::shared_ptr<Vector3> targetPos)
{
	if(!m_bInitializedStartGoal)
	{
		//printf("Initializing Start Goal X:%d Z:%d\n", currentPos.m_iX, currentPos.m_iZ);
		CleanUp();

		// Initialize start
		SearchCell start;
		start.m_iCoordinateX = m_pGameWorld->GetCellX(currentPos->m_iX);
		start.m_iCoordinateZ = m_pGameWorld->GetCellZ(currentPos->m_iZ);

		// Initialize goal
		SearchCell goal;
		goal.m_iCoordinateX = m_pGameWorld->GetCellX(targetPos->m_iX);
		goal.m_iCoordinateZ = m_pGameWorld->GetCellZ(targetPos->m_iZ);

		SetStartAndGoal(start, goal);
		m_bInitializedStartGoal = true;
		m_eState = PROCESSING;

		PrintStatus("Start Goal Initialized");
	}

	if(m_bInitializedStartGoal)
	{
		ContinuePath();
	}
}

void AStar::SetStartAndGoal(SearchCell start, SearchCell goal)
{
	//printf("Called SetStartAndGoal()\n");
	m_GoalCell = new SearchCell(goal.m_iCoordinateX, goal.m_iCoordinateZ, &goal, m_pGameWorld->GetWorldSize());

	SearchCell startCell;
	startCell.m_iCoordinateX = start.m_iCoordinateX;
	startCell.m_iCoordinateZ = start.m_iCoordinateZ;
	startCell.m_pParent = 0;
	startCell.SetId(m_pGameWorld->GetWorldSize());
	startCell.G = 0;
	startCell.H = startCell.ManhattanDistance(m_GoalCell);

	m_vOpenList.push_back(new SearchCell(startCell));
}

SearchCell *AStar::GetNextCell()
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
		m_vClosedList.push_back(nextCell);
		m_vOpenList.erase(m_vOpenList.begin() + cellIndex);
	}

	//printf("Called GetNextCell() : X:%d Z:%d\n", nextCell->m_iCoordinateX, nextCell->m_iCoordinateZ);

	return nextCell;
}

void AStar::PathOpened(int x, int z, float newCost, SearchCell *parent)
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

	for(int i = 0; i < m_vClosedList.size(); i++)
	{
		if(id == m_vClosedList[i]->m_iId)
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

void AStar::ContinuePath()
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
			m_vPathToGoal.push_back(std::shared_ptr<Vector3>(new Vector3(getPath->m_iCoordinateX * CELL_SIZE, 0, getPath->m_iCoordinateZ * CELL_SIZE)));
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

std::vector<std::shared_ptr<Vector3> > *AStar::GetPathToGoal()
{
	// Once the path to goal has been accessed the pathfinder
	// can be reset to IDLE in order to process more requests
	m_eState = IDLE;
	return &m_vPathToGoal;
}

void AStar::ResetPath()
{
	m_eState = IDLE;
	m_bInitializedStartGoal = false;
}


