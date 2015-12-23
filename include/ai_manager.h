#ifndef AI_MANAGER_H_
#define AI_MANAGER_H_

#include "../external/libraig/include/raig.h"

class AIManager
{
public:
	void ProcessRequest(int in, int out);
	void InitializePacket(raig::Packet* packet);
};

#endif /* INCLUDES_AI_MANAGER_H_ */
