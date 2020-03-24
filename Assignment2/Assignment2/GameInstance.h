#pragma once

#include "GLIncludes.h"

#include "OBJLoader.h"
#include "GameObject.h"
#include "SpaceShip.h"
#include "TextMeshLoader.h"
#include "Cube.h"

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
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;


	SpaceShip* mSpaceShip;

	Camera* mCamera;
	Light* mLight;

	Texture2D* mBgTexture;

	bool firstMouse = true;
	bool followMouse = false;

	void DrawString(const char* text, Vector3* position, Color* color);

	void DrawBackground();
	void DisableProjection();
	void EnableProjection();

	void ResetCamera();

};

