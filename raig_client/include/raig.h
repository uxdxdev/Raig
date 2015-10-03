#ifndef _RAIG_RAIG_H
#define _RAIG_RAIG_H

# define LINESIZE 80
# define HANGMAN_TCP_PORT 1068

struct Position2D
{
	int x;
	int y;
};

// interface
class IRAIG
{
public:
	virtual ~IRAIG(){};
	virtual void createMap() = 0;
	virtual Position2D readData(int x, int y) = 0;
	virtual void updateMap() = 0;
	virtual void update(int argc, char* ipAddress) = 0;
	virtual void deleteEntity() = 0;
	virtual bool isAlive() = 0;
	virtual void shutdown() = 0;
};

class RAIG: public IRAIG
{
public:
	RAIG();
	~RAIG();
	void createMap();
	Position2D readData(int x, int y);
	void updateMap();
	void update(int argc, char* ipAddress);
	void deleteEntity();
	bool isAlive();
	void shutdown();
	void cleanUp();
private:
	Position2D data;
	bool isComplete;
	bool alive;
};
#endif
