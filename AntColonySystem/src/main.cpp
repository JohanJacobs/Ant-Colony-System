
#include "AntSystem/AntSystem.h"
#include "AntColonySystem/AntColonySystem.h"

int main(int argc, char *ergv[])
{
	//AntSystem::AntSystem sim;	
	//sim.Setup(AntSystem::SimSettings(100, 100, 100));
	//sim.Run();	


	AntColonySystem::AntColonySystem sim2;
	sim2.Run(100, 100, 100);
	
	return 0;
}