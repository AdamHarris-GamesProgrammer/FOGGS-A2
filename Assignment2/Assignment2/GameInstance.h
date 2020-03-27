#pragma once

#include "GLIncludes.h"

#include "OBJLoader.h"
#include "GameObject.h"
#include "SpaceShip.h"
#include "TextMeshLoader.h"
#include "TextGameObject.h"
#include "OBJGameObject.h"
#include "Coin.h"
#include "Camera.h"
#include "Collisions.h"

#include <sstream> //Used for time text stringstream
#include <iomanip> //Used for std::setprecision
#include <functional>

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

	int menu;

	SpaceShip* mSpaceShip;

	Coin* mCoin;

	Camera* mCamera;
	Light* mLight;

	Texture2D* mBgTexture;

	Collisions* mCollisionsInstance;

	bool firstMouse = true;
	bool followMouse = false;
	
	static bool paused;
	static bool  gameOver;
	float gameTimer;
	const float gameDuration = 30.0f;

	void DrawString(const char* text, Vector2* position, Color* color);
	void DrawUI();
	void DrawBackground();
	void DisableProjection();
	void EnableProjection();

	static void PauseMenu(int option);

	std::stringstream timeText;

	int mScore = 0;

	GameInstance* instance = nullptr;
};

