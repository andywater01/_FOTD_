#include "Game.h"
#include <Windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#undef main
int main()
{
	//Create the Game
	Game game;

	//Initialize the game
	game.InitGame();
	
	PlaySound(TEXT("Back Track 1.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	//Runs the game until it returns :)
	return game.Run();
}