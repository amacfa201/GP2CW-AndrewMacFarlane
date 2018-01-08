#include "GameScript.h"
#include <iostream>
#include <string>

unsigned int myIndices[] = { 0, 1, 2 };

//Transforms to hold positions of the objects
Transform enemyTransform;
Transform playerTransform;
Transform bulletTransform;
Transform skyboxTransform;

GameScript::GameScript()
{

	_stateOfGame = GameState::PLAY;
	//Pointers 
	DisplayGame* _gameDisplay = new DisplayGame(); //create new game display
	createMesh* mesh1();
	createMesh* mesh2();
	createMesh* playerMesh();
	modelTexture* playerTexture();
	modelTexture* enemyTexture();
	modelTexture* skyboxTexture();
	modelShader* playerShader();
	modelShader* enemyShader();
	modelShader* bulletShader();
	createMesh* meshArray();
	createCamera* myCamera();
	collision = false;
}

GameScript::~GameScript()
{
	
}



void GameScript::runGame()
{
	
	startSystems(); 
	createGameLoop();
	
}

void GameScript::startSystems()
{

	_displayGame.startDisplay(); 
	
	//Initialise Sounds
	backgroundSound = _gameAudio.loadInSoundFx("..\\res\\backgroundmusic.wav");
	laserSound = _gameAudio.loadInSoundFx("..\\res\\laser2.wav");
	bangSound = _gameAudio.loadInSoundFx("..\\res\\bang.wav");
	
	//Initialise Meshes
	//firstMesh.loadInModel("..\\res\\monkey3.obj");
	//secondMesh.loadInModel("..\\res\\monkey3.obj");
	
	playerMesh.loadInModel("..\\res\\ship.obj");
	bulletMesh.loadInModel("..\\res\\bullet.obj");
	//Loads in enemy models to an array of mesh
	for (int i = 0; i <= numberOfEnemies; i++)
	{
		meshArray[i].loadInModel("..\\res\\Alien.obj");
	}



	
	//Initialise Textures
	enemyTexture.initialiseTexture("..\\res\\lizard.jpg"); 
	playerTexture.initialiseTexture("..\\res\\metal.jpg");
	bulletTexture.initialiseTexture("..\\res\\bulletTexture.jpg");
	skyboxTexture.initialiseTexture("..\\res\\space.jpg");

	//Initialise Shaders
	enemyShader.initialiseShader("..\\res\\shader"); 
	playerShader.initialiseShader("..\\res\\shader");
	bulletShader.initialiseShader("..\\res\\shader");
	skyboxShader.initialiseShader("..\\res\\shader");
	
	//Initialise Camera
	MainCamera.initialiseCamera(glm::vec3(0, 0, -5), 70.0f, (float)_displayGame.fetchWidth() / _displayGame.fetchHeight(), 0.01f, 1000.0f);
	myCounter = 5.0f;

}

void GameScript::createGameLoop()
{
	//Essentialy games update loop code in here will execute while the game is in its play state
	while (_stateOfGame != GameState::EXIT)
	{
		handleInput();
		CheckCollison();
		renderGame();
	
		
		//background music
		playGameAudio(backgroundSound, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void GameScript::handleInput()
{
	//Gets player input and responds accordingly
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) 
	{
		switch (evnt.type)
		{
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
				
				{
			case SDLK_UP:
				//changes skybox size(testing purposes)
				//skyboxSize++;
				break;
			case SDLK_DOWN:
				//changes skybox size(testing purposes)
				//skyboxSize--;
				break;

				

				case SDLK_a:
					//A - Key
					//Moves player left if within bounds of screen
					if (playerTransform.GetPos()->x < 2.5){
						MovePlayer(1);
					}
				

					break;
				case SDLK_d:
					//D - Key
					//Moves player right if within bounds of screen
					
					if (playerTransform.GetPos()->x > - 2.5) {
						MovePlayer(-1);
					}
					
					break;
				case SDLK_SPACE:
					//When space is pressed applys movement to bullet and plays the shooting sound
					bulletTransform.SetPos(glm::vec3(playerTransform.GetPos()->x, playerTransform.GetPos()->y, 0));
					firedBullet = true;
					playGameAudio(laserSound, glm::vec3(0, 0, 0));
					
					
					break;
				}
		
			break;
		case SDL_QUIT:
			_stateOfGame = GameState::EXIT;
			break;
		}
		
		
	}

	
}

bool GameScript::sphereCollision(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2)
{
	//Calculates the distance between 2 given objects using their positions and the size of their collliders
	float distance = glm::sqrt((pos2.x - pos1.x)*(pos2.x - pos1.x) + (pos2.y - pos1.y)*(pos2.y - pos1.y) + (pos2.z - pos1.z)*(pos2.z - pos1.z));


	//Returns appropriate bool depending on whether collison has occurred
	if (distance < (rad1 + rad2))
	{
		collision = true;
		return true;
	}
	else
	{
		return false;
	}
}

void GameScript::playGameAudio(unsigned int Source, glm::vec3 pos)
{
	//Handles playing of audio depening on audio state
	ALint state;
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	/*
	Possible values of state
	AL_INITIAL
	AL_STOPPED
	AL_PLAYING
	AL_PAUSED
	*/
	if (AL_PLAYING != state)
	{
		_gameAudio.playSoundFx(Source, pos);
	}
}

void GameScript::renderGame()
{

	_displayGame.wipeDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	//Method calls for rendering various objects
	DrawEnemies();
	DrawPlayer();
	DrawBullet();
	CreateSkybox();

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_displayGame.changeBuffers();
}
void GameScript::MovePlayer(int direction)
{
	//manipulates player transform based up input which defines the direction of movement
	playerTransform.SetPos(glm::vec3(playerTransform.GetPos()->x + .25 * direction, -2, 0));

}

void GameScript::DrawBullet()
{
	
	if (firedBullet)
	{
		//Applys movement to bullet transforms
		bulletTransform.SetPos(glm::vec3(bulletTransform.GetPos()->x, bulletTransform.GetPos()->y+0.05, 0));
		bulletTransform.SetRot(glm::vec3(0.0, 0.0, 0));
		bulletTransform.SetScale(glm::vec3(0.1, 0.1, 0.1));

		//Various function calls to allow rendering of bullet
		bulletShader.BindShaders(); //Binds shaders
		bulletShader.Update(bulletTransform, MainCamera); //updates position of bullets relevant to main camera
		bulletTexture.BindTexture(0); // Applys texture to mesh
		bulletMesh.DrawMesh(); // Draws mesh
		bulletMesh.checkSphereStatus(*bulletTransform.GetPos(), 0.3f); // adds collider to mesh with radius 0.3
		
		
	}

}

void GameScript::DrawEnemies()
{
	//Loop to spawn enemies across screen using varying x co-ordinate
	float xCoord = -2;
	for (int i = 0; i <= numberOfEnemies; i++)
	{
		
		//Manipulates transforms approprtiatley 
		enemyTransform.SetPos(glm::vec3(xCoord, 2.0, 0.0));
		enemyTransform.SetRot(glm::vec3(180.0, -sinf(counter), 0.0)); //spins enemies using counter value and sign function
		enemyTransform.SetScale(glm::vec3(0.15, 0.15, 0.15));
		
		enemyShader.BindShaders(); //Binds shaders
		enemyShader.Update(enemyTransform, MainCamera); //Updates position of the enemies relative to the camera
		enemyTexture.BindTexture(0); // applys texture to mesh
		meshArray[i].DrawMesh(); // Draws mesh
		
		meshArray[i].checkSphereStatus(*enemyTransform.GetPos(), 0.3f); // adds collider to mesh with radius 0.3
		xCoord += 0.75; // increases x co-ordinate in order to spawn enemies in a line
		
	}
	counter += 0.0005f;
}
void GameScript::DrawPlayer()
{
	//Sets player transforms
	playerTransform.SetRot(glm::vec3(0.0, 0, 0));
	playerTransform.SetScale(glm::vec3(0.25, 0.25, 0.25));

	
	playerShader.BindShaders(); //Binds Shaders 
	playerShader.Update(playerTransform, MainCamera); // Updates player position relative to camer
	playerTexture.BindTexture(0); //Binds Texture tp mesh
	playerMesh.DrawMesh(); //draws player mesh
	
	
	playerMesh.checkSphereStatus(*playerTransform.GetPos(), 0.3f); //Adds collider to player mesh
}
void GameScript::CheckCollison()
{
	//Checks for collision between bullet and array of enemies and deletes enemy when hit
	for (int i = 0; i < 6; i++) {
		if (sphereCollision(bulletMesh.getSpherePos(), bulletMesh.getSphereRadius(), meshArray[i].getSpherePos(), meshArray[i].getSphereRadius())) {
			meshArray[i].~createMesh(); //Deletes enemy
			playGameAudio(bangSound, glm::vec3(0.0f, 0.0f, 0.0f)); //Plays explosion sound
			//firedBullet = false;
		}
	}
}


void GameScript::CreateSkybox() {
	//Sets transform of skybox
	skyboxTransform.SetPos(glm::vec3(playerTransform.GetPos()->x, playerTransform.GetPos()->y, playerTransform.GetPos()->z));

	skyboxShader.BindShaders(); //Binds shader to skybox
	skyboxShader.Update(skyboxTransform, MainCamera); // Updates position of skybox relative to the camera
	skyboxTexture.BindTexture(0); //Binds texture to skybox


	//Draws a face of the skybox by creating vertices and assigns texture coordinates
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(skyboxSize / 2, -skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-skyboxSize / 2, -skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-skyboxSize / 2, skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(0, 1);
	glVertex3f(skyboxSize / 2, skyboxSize / 2, skyboxSize / 2);
	glEnd();

	


	skyboxShader.BindShaders(); //Binds shader to skybox
	skyboxShader.Update(skyboxTransform, MainCamera); // Updates position of skybox relative to the camera
	skyboxTexture.BindTexture(0); //Binds texture to skybox
	
	
	//Draws a face of the skybox by creating vertices and assigns texture coordinates
	glBegin(GL_QUADS);
	
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxSize / 2, -skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-skyboxSize / 2, -skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-skyboxSize / 2, skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxSize / 2, skyboxSize / 2, skyboxSize / 2);
	glEnd();

	skyboxShader.BindShaders(); //Binds shader to skybox
	skyboxShader.Update(skyboxTransform, MainCamera); // Updates position of skybox relative to the camera
	skyboxTexture.BindTexture(0); //Binds texture to skybox

	
	//Draws a face of the skybox by creating vertices and assigns texture coordinates
	glBegin(GL_QUADS);
	
	glTexCoord2f(1, 0);
	glVertex3f(skyboxSize / 2, skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxSize / 2, skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxSize / 2, -skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxSize / 2, -skyboxSize / 2, -skyboxSize / 2);
	glEnd();

	skyboxShader.BindShaders(); //Binds shader to skybox
	skyboxShader.Update(skyboxTransform, MainCamera); // Updates position of skybox relative to the camera
	skyboxTexture.BindTexture(0); //Binds texture to skybox

	
	//Draws a face of the skybox by creating vertices and assigns texture coordinates
	glBegin(GL_QUADS);
	
	glTexCoord2f(0, 0);
	glVertex3f(skyboxSize / 2, -skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(1, 0);
	glVertex3f(skyboxSize / 2, -skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxSize / 2, skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(0, 1);
	glVertex3f(skyboxSize / 2, skyboxSize / 2, -skyboxSize / 2);
	glEnd();

	skyboxShader.BindShaders(); //Binds shader to skybox
	skyboxShader.Update(skyboxTransform, MainCamera); // Updates position of skybox relative to the camera
	skyboxTexture.BindTexture(0); //Binds texture to skybox

	
	//Draws a face of the skybox by creating vertices and assigns texture coordinates
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0);
	glVertex3f(skyboxSize / 2, skyboxSize / 2, -skyboxSize / 2);//
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxSize / 2, skyboxSize / 2, -skyboxSize / 2);//
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxSize / 2, skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxSize / 2, skyboxSize / 2, skyboxSize / 2);
	glEnd();

	skyboxShader.BindShaders(); //Binds shader to skybox
	skyboxShader.Update(skyboxTransform, MainCamera); // Updates position of skybox relative to the camera
	skyboxTexture.BindTexture(0); //Binds texture to skybox
	
	//Draws a face of the skybox by creating vertices and assigns texture coordinates
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxSize / 2, -skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxSize / 2, -skyboxSize / 2, -skyboxSize / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxSize / 2, -skyboxSize / 2, skyboxSize / 2);
	glTexCoord2f(1, 0);
	glVertex3f(skyboxSize / 2, -skyboxSize / 2, skyboxSize / 2);
	glEnd();
}