#include "Raig.h" // API for developers

extern "C" {
	#include "../libsocket/Sockets.h"
}

using namespace raig;

// RaigImpl class declaration
class Raig::RaigImpl
{
public:
	void connect();
	void sendData(char* dataString);
	void sendData(int value);
	void update();

	// Private members and functions
	void cleanUp();
	bool isComplete;
	bool alive;

	int iSocketFileDescriptor;
	char * strServerIPAddress;
	struct Address sAddress;
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

void Raig::connect()
{
	m_Impl->connect();
}

void Raig::sendData(char* dataString)
{
	m_Impl->sendData(dataString);
}

void Raig::sendData(int value)
{
	m_Impl->sendData(value);
}

void Raig::update()
{
	m_Impl->update();
}

/*
 * RaigImpl implementation
 */
void Raig::RaigImpl::connect()
{
	std::cout << "Raig::RaigImpl::connect()" << std::endl;
	/*
	strServerIPAddress = "127.0.0.1";

	iSocketFileDescriptor = Socket(AF_INET, SOCK_STREAM, 0);

	Address(AF_INET, (struct Address*) &sAddress, strServerIPAddress, HANGMAN_TCP_PORT);

	Connect(iSocketFileDescriptor, (struct sockaddr*) &sAddress.m_sAddress, sizeof(sAddress.m_sAddress));
	*/
}

void Raig::RaigImpl::sendData(char* dataString)
{
	std::cout << "Raig::RaigImpl::sendData() : " << dataString << std::endl;
	//multiplexStdinFileDescriptor(stdin, iSocketFileDescriptor);
}

void Raig::RaigImpl::sendData(int value)
{
	std::cout << "Raig::RaigImpl::sendData() : " << value << std::endl;
}

void Raig::RaigImpl::update()
{
	std::cout << "Raig::RaigImpl::update()" << std::endl;
}

void Raig::RaigImpl::cleanUp()
{
	std::cout << "Raig::RaigImpl::cleanUp()" << std::endl;
}
