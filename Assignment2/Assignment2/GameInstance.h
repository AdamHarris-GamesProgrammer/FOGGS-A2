#pragma once

#include "GLIncludes.h"

#include "OBJLoader.h"

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
	OBJLoader loader;

	Camera* mCamera;
	Light* mLight;

	class  Texture2D* texture;

	float rotation = 0.0f;
};

