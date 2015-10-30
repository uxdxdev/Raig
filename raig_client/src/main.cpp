#include "../../libraig/Raig.h"

int main(int argc, char* argv[])
{
	raig::Raig* raig = new raig::Raig();
	raig->createMap();
	raig->update();
	delete raig;
	return 0;
}
