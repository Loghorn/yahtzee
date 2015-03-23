#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "../Yahtzee/Yahtzee.h"
#include "../Yahtzee/GameConfiguration.h"
#include "../Yahtzee/YahtzeeWriter.h"
#include "ConsoleWriter.h"

int main()
{
	ConsoleWriter consoleYahtzee;
	consoleYahtzee.run();
}
