
#include "AntSystem.h"

int main(int argc, char *ergv[])
{
	AntSystem::AntSystem sim;	
	sim.Setup(AntSystem::SimSettings(20,50, 30));
	sim.Run();	
	
	return 0;
}