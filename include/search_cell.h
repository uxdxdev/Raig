#ifndef SEARCH_CELL_H
#define SEARCH_CELL_H

#include <math.h>
#include <stdio.h>

class SearchCell{
public:
	int m_iCoordinateX;
	int m_iCoordinateZ;
	int m_iId;
	SearchCell *m_pParent;
	float G;
	float H;

	SearchCell()
	{
		m_iCoordinateX = 0;
		m_iCoordinateZ = 0;
		m_iId = 0;
		m_pParent = 0;
		G = 0;
		H = 0;
	}

	SearchCell(int x, int z, SearchCell *parent = 0, int worldSize = 0)
	{
		m_iCoordinateX = x;
		m_iCoordinateZ = z;
		m_iId = (z * worldSize + x);
		m_pParent = parent;
		G = 0;
		H = 0;
	}

	virtual ~SearchCell()
	{
		printf("dtor SearchCell() ID: %d\n", m_iId);
	}

	float GetF()
	{
		return G + H;
	}

	void SetId(int id)
	{
		m_iId = (m_iCoordinateZ * id + m_iCoordinateX);
	}

	float ManhattanDistance(SearchCell *nodeEnd)
	{
		float x = (float)(fabs((float)this->m_iCoordinateX - nodeEnd->m_iCoordinateX));
		float z = (float)(fabs((float)this->m_iCoordinateZ - nodeEnd->m_iCoordinateZ));

		return x + z;
	}
};

#endif
