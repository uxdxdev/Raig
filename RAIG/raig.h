#ifndef _RAIG_RAIG_H
#define _RAIG_RAIG_H

// included for testing
#include <iostream>

struct Position2D{
	int x;
	int y;
};

// interface
class IRAIG{
public:
	virtual ~IRAIG(){};
	virtual void createMap() = 0;
	virtual Position2D readData(int x, int y) = 0;
	virtual void updateMap() = 0;
	virtual void update() = 0;
	virtual void deleteEntity() = 0;
	virtual bool isAlive() = 0;
};

class RAIG : public IRAIG{
public:
	RAIG()
	{
		isComplete = false;
		alive = true;
	};
	~RAIG(){};
	void createMap(){ std::cout << "map created" << std::endl; }
	Position2D readData(int x, int y)
	{
		data.x = x;
		data.y = y;
		return data;
	}
	void updateMap(){ std::cout << "map updated" << std::cout; }
	void update()
	{
		std::cout << "in raig update loop..." << std::endl;
		std::cout << "check if there are any messages from the server using select()" << std::endl;
		std::cout << "if there are, copy data into raig buffer and set date waiting flag to true" << std::endl;

		// shutdown the raig client
		alive = false;
	}
	void deleteEntity(){ std::cout << "entity deleted" << std::endl; }
	bool isAlive(){ return alive; }
private:
	Position2D data;
	bool isComplete;
	bool alive;
};
#endif
