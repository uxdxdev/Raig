#include "Raig.h"

using namespace raig;

int main()
{
	Raig* raig = new Raig();
	raig->connect("127.0.0.1");
	raig->sendData((char*)"Hello there");
	return 0;
}
