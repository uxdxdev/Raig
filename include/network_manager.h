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
#ifndef _INCLUDE_NETWORK_MANAGER_H_
#define _INCLUDE_NETWORK_MANAGER_H_

#include <memory>

extern "C" {
	#include "../external/libsocket/include/socket.h"
}

class AIManager;

class NetworkManager
{
public:
	NetworkManager();

	~NetworkManager();

	static void SignalHandler(int signalNumber);

	void Signal(int signalNumber);

	void Start();

private:
	void Init();

	void CleanUp();

	// AI algorithm Manager
	std::unique_ptr<AIManager> m_AIManager;

	// Server listen file descriptor
	int m_iListenSocketFileDescriptor;

	// Client address structure
	struct Address m_sAddress;

	// New process id for use with fork
	pid_t m_ChildProcessID;

	// New connection file descriptor
	int m_iConnfd;

	socklen_t m_ClientLen;
};

#endif
