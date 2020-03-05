#include "GameInstance.h"
#include "GlutCallbacks.h"
#include "Constants.h"

#include <iostream>

GameInstance::GameInstance(int argc, char* argv[])
{
	InitOpenGL(argc, argv);
	InitObjects();

	glutMainLoop();
}

GameInstance::~GameInstance()
{

}

void GameInstance::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glFlush();
	glutSwapBuffers();
}

void GameInstance::Update()
{
	
}

void GameInstance::InitOpenGL(int argc, char* argv[])
{
	GLUTCallback::Init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("FOGGS Assignment 2 - Adam Harris");
	glutDisplayFunc(GLUTCallback::Display);
	glutTimerFunc(REFRESH_RATE, GLUTCallback::Timer, REFRESH_RATE);
}

void GameInstance::InitObjects()
{

}
