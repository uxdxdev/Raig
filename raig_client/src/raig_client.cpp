#include "../../libraig/Raig.h"

using namespace raig;

int main()
{
	Raig* raig = new Raig();
	raig->connect();
	raig->sendData((char*)"data string");

	return 0;
}
