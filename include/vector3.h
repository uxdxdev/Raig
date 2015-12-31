#ifndef VECTOR3
#define VECTOR3

struct Vector3{
public:
	int m_iX;
	int m_iY;
	int m_iZ;

	Vector3()
	{
		m_iX = 0;
		m_iY = 0;
		m_iZ = 0;
	}

	Vector3(int x, int y, int z)
	{
		m_iX = x;
		m_iY = y;
		m_iZ = z;
	}
};

#endif
