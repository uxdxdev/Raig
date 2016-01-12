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
#ifndef _INCLUDE_SEARCH_CELL_H
#define _INCLUDE_SEARCH_CELL_H

#include <math.h>

class SearchCell{
public:
	int m_iCoordinateX;
	int m_iCoordinateZ;
	int m_iId;
	SearchCell *m_pParent;
	float m_fCostSoFarG;
	float m_fEstimatedCostToGoalH;

	SearchCell()
	{
		m_iCoordinateX = 0;
		m_iCoordinateZ = 0;
		m_iId = 0;
		m_pParent = 0;
		m_fCostSoFarG = 0;
		m_fEstimatedCostToGoalH = 0;
	}

	SearchCell(int x, int z, SearchCell *parent = 0, int worldSize = 0)
	{
		m_iCoordinateX = x;
		m_iCoordinateZ = z;
		m_iId = (z * worldSize + x);
		m_pParent = parent;
		m_fCostSoFarG = 0;
		m_fEstimatedCostToGoalH = 0;
	}

	float GetEstimatedTotalCostF()
	{
		return m_fCostSoFarG + m_fEstimatedCostToGoalH;
	}

	void SetId(int worldSize)
	{
		m_iId = (m_iCoordinateZ * worldSize + m_iCoordinateX);
	}

	float ManhattanDistance(SearchCell *nodeEnd)
	{
		float x = (float)(fabs((float)this->m_iCoordinateX - nodeEnd->m_iCoordinateX));
		float z = (float)(fabs((float)this->m_iCoordinateZ - nodeEnd->m_iCoordinateZ));

		return x + z;
	}
};

#endif
