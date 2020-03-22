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

	float currentFrame = (float)glutGet(GLUT_ELAPSED_TIME) / 1000;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	std::cout << deltaTime << std::endl;


	gluLookAt(mCamera->mRight.x, mCamera->mRight.y, mCamera->mRight.z, mCamera->mFront.x, mCamera->mFront.y, mCamera->mFront.z, mCamera->mUp.x, mCamera->mUp.y, mCamera->mUp.z);

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(mLight->ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(mLight->diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(mLight->specular.x));

	mSpaceShip->Update();




	glTranslatef(mCamera->mPosition.x, mCamera->mPosition.y, mCamera->mPosition.z); 

	glutPostRedisplay();
}

void GameInstance::Keyboard(unsigned char key, int x, int y)
{
	mSpaceShip->PollInput(key, x, y);

	float cameraSpeed = 2.5f * deltaTime;
	if (key == 'w') {
		mCamera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (key == 's') {
		mCamera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (key == 'a') {
		mCamera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (key == 'd') {
		mCamera->ProcessKeyboard(RIGHT, deltaTime);
	}


}

void GameInstance::KeyboardUp(unsigned char key, int x, int y)
{
	mSpaceShip->KeyUp(key, x, y);
}

void GameInstance::ActiveMotion(int x, int y)
{
	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xOffset = x - lastX;
	float yOffset = lastY - y; //Opposite as coordinate system is from top left
	lastX = x;
	lastY = y;
	
	mCamera->ProcessInput(xOffset, yOffset);
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
	glutPassiveMotionFunc(GLUTCallback::ActiveMouseMotion);
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
	mCamera = new Camera(Vector3(0.0f,0.0f,-35.0f));


	mLight = new Light();

	mSpaceShip = new SpaceShip(std::string("Assets/test3.obj"));


}
