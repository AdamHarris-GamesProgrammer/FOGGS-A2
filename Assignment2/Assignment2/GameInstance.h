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
	void ActiveMotion(int x, int y);

	void InitOpenGL(int argc, char* argv[]);
	void InitObjects();

	float lastX = 400.0f, lastY = 400.0f;
	float yaw = -90.0f, pitch;

private:
	SpaceShip* mSpaceShip;

	Camera* mCamera;
	Light* mLight;

	bool firstMouse = true;
};

