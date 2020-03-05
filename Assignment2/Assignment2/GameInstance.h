#pragma once

#include <Windows.h>
#include "gl/GL.h"
#include "gl/GLU.h"
#include "GL\freeglut.h"

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

};

