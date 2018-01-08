#pragma once
#include <SDL/SDL.h>
#include "DisplayGame.h" 
#include <GL\glew.h>
#include "modelShader.h"
#include "createMesh.h"
#include "modelTexture.h"
#include "transform.h"
#include <list>;
#include "gameAudio.h"

//The Script previously known as: "MainGame.h"
enum class GameState{PLAY, EXIT};

class GameScript
{
public:
	GameScript();
	~GameScript();

	void runGame();

private:

	void startSystems();
	void handleInput();
	void createGameLoop();
	void renderGame();
	bool sphereCollision(glm::vec3 pos1, float rad1, glm::vec3 pos2, float rad2);
	void playGameAudio(unsigned int Source, glm::vec3 pos);
	void inputs();
	void MovePlayer(int i);
	void DrawBullet();
	void DrawPlayer();
	void DrawEnemies();
	void CheckCollison();
	void CreateSkybox();


	DisplayGame _displayGame;
	GameState _stateOfGame;
	bool collision;
	bool firedBullet;
	createMesh firstMesh;
	createMesh secondMesh;	
	createMesh playerMesh;
	createMesh bulletMesh;
	createMesh meshArray[5];
	
	//Transform 
	//std::list<createMesh> EnemyList;
	
	std::list<createMesh> bulletMeshes;
	bool renderBullet[30];
	bool renderEnemy[5];
	bool doOnce = false;
	std::list<Transform> bulletTransforms;
	int numberOfEnemies = 5;
	int numberOfBullets=0;
	createMesh meshesForLoad[3];
	
	createCamera MainCamera;
	modelTexture enemyTexture;
	modelTexture playerTexture;
	modelTexture bulletTexture;
	modelTexture skyboxTexture;

	modelShader enemyShader;
	modelShader playerShader;
	modelShader bulletShader;
	modelShader skyboxShader;


	float myCounter;
	float movement = 0;
	float counter = 0;
	float skyboxSize = -9;

	gameAudio _gameAudio;

	unsigned int backgroundSound;
	unsigned int laserSound;
	unsigned int bangSound;
};

