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

	DisableProjection();

	DrawBackground();

	DrawString("Score: 0", &Vector3(0.0f, 0.9f, 0.0f), &Color(1.0f, 0.0f, 0.0f));

	EnableProjection();


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


	gluLookAt(mCamera->eye.x, mCamera->eye.y, mCamera->eye.z, mCamera->center.x, mCamera->center.y, mCamera->center.z, mCamera->up.x, mCamera->up.y, mCamera->up.z);

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(mLight->ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(mLight->diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(mLight->specular.x));

	mSpaceShip->Update();

	glTranslatef(mCamera->position.x, mCamera->position.y, mCamera->position.z);

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
	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xOffset = x - lastX;
	float yOffset = lastY - y; //Opposite as coordinate system is from top left
	lastX = x;
	lastY = y;


	const float sensitivity = 0.5f;
	yOffset *= sensitivity;
	xOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	Vector3 direction;
	direction.x = cos(Math::DegreeToRadians(yaw)) * cos(Math::DegreeToRadians(pitch));
	direction.y = sin(Math::DegreeToRadians(pitch));
	direction.z = sin(Math::DegreeToRadians(yaw)) * cos(Math::DegreeToRadians(pitch));


	mCamera->center = Math::Normalise(direction);
}

void GameInstance::InitOpenGL(int argc, char* argv[])
{
	GLUTCallback::Init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("FOGGS Assignment 2 - Adam Harris");

	glutKeyboardFunc(GLUTCallback::Keyboard);
	glutKeyboardUpFunc(GLUTCallback::KeyboardUp);
	glutPassiveMotionFunc(GLUTCallback::ActiveMouseMotion);
	glutTimerFunc(REFRESH_RATE, GLUTCallback::Timer, REFRESH_RATE);
	glutDisplayFunc(GLUTCallback::Display);

	glMatrixMode(GL_PROJECTION);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	gluPerspective(45, 1, 1, 200);


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
	mCamera->position = Vector3(0.0f, 0.0f, -35.0f);


	mLight = new Light();

	mSpaceShip = new SpaceShip(std::string("Assets/test3.obj"));

	mBgTexture = new Texture2D();
	mBgTexture->LoadBMP((char*)"Assets/BgTexture.bmp");
}

void GameInstance::DrawString(const char* text, Vector3* position, Color* color)
{
	glEnable(GL_LIGHTING);
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	glRasterPos2f(position->x, position->y);
	glColor3f(color->r, color->g, color->b);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);
	glPopMatrix();
	glPopAttrib();
	glDisable(GL_LIGHTING);
}

void GameInstance::DrawBackground()
{
	glBindTexture(GL_TEXTURE_2D, mBgTexture->GetID());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_COORD_ARRAY);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2i(0, 0);
	glVertex2i(-1, -1);
	glTexCoord2i(1, 0);
	glVertex2i(1, -1);
	glTexCoord2i(1, 1);
	glVertex2i(1, 1);
	glTexCoord2i(0, 1);
	glVertex2i(-1, 1);
	glEnd();
	glDisable(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameInstance::DisableProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}

void GameInstance::EnableProjection()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
