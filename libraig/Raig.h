#ifndef _RAIG_RAIG_H
#define _RAIG_RAIG_H

#include <iostream>
#include <memory.h>

#define LINESIZE 80
#define HANGMAN_TCP_PORT 1076

namespace raig{

typedef struct Position2D
{
	int x;
	int y;
} Position2D;

class Raig
{
public:
	Raig();
	~Raig();
	void createMap();
	Position2D readData(int x, int y);
	void updateMap();
	void update();
	void deleteEntity();
	bool isAlive();
	void shutdown();
private:
	class RaigImpl; // Forward declaration
	std::unique_ptr<RaigImpl> m_Impl; // Raig implementation using auto_ptr
};

}
#endif
