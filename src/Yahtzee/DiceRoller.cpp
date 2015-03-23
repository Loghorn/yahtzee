#include "DiceRoller.h"
#include <chrono>

using namespace std;

DiceRoller::DiceRoller(unsigned short pMaxDiceValue) : maxDiceValue(pMaxDiceValue), rengine((unsigned int) time(0))
{
}

void DiceRoller::Roll(vector<Die>& dice)
{
	uniform_int_distribution<unsigned short> rdist(1, maxDiceValue); 
	
	for (auto& die : dice)
	{
		if (!die.hold)
			die.value = rdist(rengine);
	}
}
