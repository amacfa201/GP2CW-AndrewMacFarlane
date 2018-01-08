#include "DisplayGame.h"



DisplayGame::DisplayGame()
{
	_windowForGame = nullptr; //initialise to generate null access violation for debugging. 
	_widthOfScreen = 1024; //set screen width
	_heightOfScreen = 768;  //set screen height

	
	
}

DisplayGame::~DisplayGame()
{
	//Destructor
	SDL_GL_DeleteContext(openGLContext); //Deletes GL context
	SDL_DestroyWindow(_windowForGame); //Deletes Game window
	SDL_Quit(); //Quits sdl
}

//Gets window sizes (height and width)
float DisplayGame::fetchWidth() { return _widthOfScreen; } 
float DisplayGame::fetchHeight() { return _heightOfScreen; }



void DisplayGame::returnError(std::string errorString)
{
	//Returns errors in case of issues
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void DisplayGame::changeBuffers()
{
	SDL_GL_SwapWindow(_windowForGame); //swap buffers
}

void DisplayGame::startDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	_windowForGame = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)_widthOfScreen, (int)_heightOfScreen, SDL_WINDOW_OPENGL); // create window

	if (_windowForGame == nullptr)
	{
		returnError("window failed to create");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(_windowForGame);

	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		
		returnError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera
	glCullFace(GL_FRONT); // Sets face culling to the front, yielded best result

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}


void DisplayGame::wipeDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

