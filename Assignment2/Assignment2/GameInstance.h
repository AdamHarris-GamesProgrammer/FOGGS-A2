#pragma once

#include <Windows.h>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "GL\freeglut.h"

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

	float rotation = 0.0f;
};

