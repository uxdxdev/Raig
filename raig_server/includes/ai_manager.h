#ifndef INCLUDES_AI_MANAGER_H_
#define INCLUDES_AI_MANAGER_H_

#include "../../libraig/raig.h"

class AIManager
{
public:
	void ProcessRequest(int in, int out);
	void InitializePacket(raig::Packet* packet);
};

#endif /* INCLUDES_AI_MANAGER_H_ */
