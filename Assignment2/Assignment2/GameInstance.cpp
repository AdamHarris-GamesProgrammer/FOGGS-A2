#include "GameInstance.h"
#include "GlutCallbacks.h"
#include "Constants.h"


#include <iostream>
#include "GraphicsStructures.h"
#include "BMP.h"
#include "Timer.h"

#include "Texture2D.h"

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

	mSpaceShip->Render();

	glFlush();
	glutSwapBuffers();
}

void GameInstance::Update()
{
	glLoadIdentity();
	gluLookAt(mCamera->eye.x, mCamera->eye.y, mCamera->eye.z, mCamera->center.x, mCamera->center.y, mCamera->center.z, mCamera->up.x, mCamera->up.y, mCamera->up.z);

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(mLight->ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(mLight->diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(mLight->specular.x));

	mSpaceShip->Update();

	glTranslatef(0.0f, 0.0f, -35.0f); 

	glutPostRedisplay();
}

void GameInstance::Keyboard(unsigned char key, int x, int y)
{
	mSpaceShip->PollInput(key, x, y);
}

void GameInstance::KeyboardUp(unsigned char key, int x, int y)
{
	mSpaceShip->KeyUp(key, x, y);
}

void GameInstance::ActiveMotion(int x, int y)
{
	if (x != 0) {
		mCamera->eye.x += 0.5f;
	}

	if (y != 0) {
		mCamera->eye.y += 0.5f;
	}
}

void GameInstance::InitOpenGL(int argc, char* argv[])
{
	GLUTCallback::Init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("FOGGS Assignment 2 - Adam Harris");

	glutKeyboardFunc(GLUTCallback::Keyboard);
	glutKeyboardUpFunc(GLUTCallback::KeyboardUp);
	glutMotionFunc(GLUTCallback::ActiveMouseMotion);
	glutTimerFunc(REFRESH_RATE, GLUTCallback::Timer, REFRESH_RATE);
	glutDisplayFunc(GLUTCallback::Display);

	glMatrixMode(GL_PROJECTION);

	glViewport(0, 0, 800, 800);
	gluPerspective(45, 1, 1, 1000);


	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMAL_ARRAY);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glCullFace(GL_BACK);
}

void GameInstance::InitObjects()
{
	mCamera = new Camera();
	mCamera->eye.z = 1.0f;
	mCamera->up.y = 1.0f;

	mLight = new Light();

	mSpaceShip = new SpaceShip(std::string("Assets/test3.obj"));


}
