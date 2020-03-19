#pragma once

#include "GLIncludes.h"

#include "OBJLoader.h"
#include "GameObject.h"

class GameInstance
{
public:
	GameInstance(int argc, char* argv[]);
	~GameInstance();

	void Render();
	void Update();
	
	void InitOpenGL(int argc, char* argv[]);
	void InitObjects();

private:
	GameObject* mSpaceShip;

	Camera* mCamera;
	Light* mLight;
};

