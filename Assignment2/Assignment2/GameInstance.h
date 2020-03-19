#pragma once

#include "GLIncludes.h"

#include "OBJLoader.h"
#include "GameObject.h"
#include "SpaceShip.h"

class GameInstance
{
public:
	GameInstance(int argc, char* argv[]);
	~GameInstance();

	void Render();
	void Update();
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);

	void InitOpenGL(int argc, char* argv[]);
	void InitObjects();

private:
	SpaceShip* mSpaceShip;

	Camera* mCamera;
	Light* mLight;
};

