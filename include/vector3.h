#ifndef VECTOR3
#define VECTOR3

struct Vector3{
	int m_iX;
	int m_iY;
	int m_iZ;

	Vector3(int x, int y, int z)
	{
		m_iX = x;
		m_iY = y;
		m_iZ = z;
	}
};

#endif VECTOR3
