#include <stdio.h> // perror, fprintf, stderr, printf
#include <cstdlib> // added for use with c code in c++
#include <netdb.h> // gethostbyname, hostent, sockaddr_in, AF_INET, SOCK_STREAM, socket(),
#include <cstring> // memcpy
#include <unistd.h> // read(), write()
#include "Raig.h" // API for developers

using namespace raig;

// RaigImpl class declaration
class Raig::RaigImpl
{
public:
	void createMap();
	Position2D readData(int x, int y);
	void updateMap();
	void update();
	void deleteEntity();
	bool isAlive();
	void shutdown();
	void cleanUp();

	Position2D data;
	bool isComplete;
	bool alive;
};

/*
 *  Raig class implementation
 */
Raig::Raig() : m_Impl(new RaigImpl())
{
	m_Impl->isComplete = false;
	m_Impl->alive = true;
}

Raig::~Raig()
{
	m_Impl->cleanUp();
}

void Raig::createMap()
{
	m_Impl->createMap();
}

Position2D Raig::readData(int x, int y)
{
	return m_Impl->readData(x, y);
}

void Raig::updateMap()
{
	m_Impl->updateMap();
}

void Raig::update()
{
	m_Impl->update();
}

void Raig::deleteEntity()
{
	m_Impl->deleteEntity();
}

bool Raig::isAlive()
{
	return m_Impl->isAlive();
}

void Raig::shutdown()
{
	m_Impl->shutdown();
}

/*
 * RaigImpl implementation
 */
void Raig::RaigImpl::createMap()
{
	std::cout << "Raig::RaigImpl::createMap()" << std::endl;
}

Position2D Raig::RaigImpl::readData(int x, int y)
{
	std::cout << "Raig::RaigImpl::readData(x, y)" << std::endl;
	data.x = x;
	data.y = y;
	return data;
}

void Raig::RaigImpl::updateMap()
{
}

void Raig::RaigImpl::update()
{
	std::cout << "Raig::RaigImpl::update()" << std::endl;
}

void Raig::RaigImpl::deleteEntity()
{
	std::cout << "Raig::RaigImpl::deleteEntity()" << std::endl;
}

bool Raig::RaigImpl::isAlive()
{
	std::cout << "Raig::RaigImpl::isAlive() : %s" << ((alive) ? "Alive" : "Dead") << std::endl;
	return alive;
}

void Raig::RaigImpl::shutdown()
{
	std::cout << "Raig::RaigImpl::shutdown()" << std::endl;
	alive = false;
}

void Raig::RaigImpl::cleanUp()
{
	std::cout << "Raig::RaigImpl::cleanUp()" << std::endl;
}
