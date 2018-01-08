#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <string>
using namespace std;

//The Script previously known as: "Display.h"
class DisplayGame
{
public:
	DisplayGame();
	~DisplayGame();
	void startDisplay();
	void changeBuffers();
	void wipeDisplay(float r, float g, float b, float);

	float fetchWidth();
	float fetchHeight();


private:

	void returnError(std::string errorString);
	SDL_GLContext openGLContext;
	SDL_Window* _windowForGame; //holds pointer to out window
	int _widthOfScreen;
	int _heightOfScreen;
};

