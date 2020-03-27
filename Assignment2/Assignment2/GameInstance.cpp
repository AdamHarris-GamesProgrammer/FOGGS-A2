#include "GameInstance.h"
#include "GlutCallbacks.h"
#include "Constants.h"


#include <iostream>
#include <algorithm>
#include "GraphicsStructures.h"
#include "BMP.h"
#include "Timer.h"

#include "Texture2D.h"

GameInstance::GameInstance(int argc, char* argv[])
{
	instance = this;

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

	//Background
	DrawBackground();

	//Mid ground Objects
	mCoin->Render();
	mSpaceShip->Render();

	//Foreground
	DrawUI();

	glFlush();
	glutSwapBuffers();
}

void GameInstance::Update()
{
	//Calculating Delta Time
	float currentFrame = (float)glutGet(GLUT_ELAPSED_TIME) / 1000;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (!paused) {
		if (!gameOver) {
			gameTimer -= deltaTime;
			if (gameTimer <= 0.0f) {
				gameTimer = 0.0f;
				gameOver = true;
			}
		}

		if (!gameOver) {
			mSpaceShip->Update();
			mCoin->Update();

			if (mCollisionsInstance->CollisionCheck(mSpaceShip->GetBox(), mCoin->GetBox())) {
				mCoin->GeneratePosition();
				mScore++;
			}
		}
	}


	glLoadIdentity();

	mCamera->Update();

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(mLight->ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(mLight->diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(mLight->specular.x));
	

	glutPostRedisplay();
}

void GameInstance::Keyboard(unsigned char key, int x, int y)
{
	if (!gameOver) {
		mSpaceShip->PollInput(key, x, y);
	}
	else
	{
		//Q resets the game
		if (key == 'q') {
			gameTimer = gameDuration;
			mScore = 0;
			gameOver = false;

			mCoin->GeneratePosition();
			mSpaceShip->SetPosition(Vector3());
		}
	}

	//Key 27 is Esc, allows player to quit
	if (key == 27) {
		exit(0);
	}

	//F toggles camera movement 
	if (key == 'f') {
		followMouse = !followMouse; 
	}

	//Resets camera values
	if (key == 'r') {
		mCamera->ResetCamera();
		followMouse = false;
	}

	if (key == 'p') {
		paused = !paused;
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

	if (followMouse) {
		mCamera->ProcessInput(xOffset, yOffset);
	}

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
	mCamera = new Camera(Vector3(0.0f, 0.0f, -35.0f), Vector3(0.0f,0.0f,1.0f), Vector3(), Vector3(0.0f,1.0f,0.0f));
	mCollisionsInstance = new Collisions();

	mLight = new Light();

	mSpaceShip = new SpaceShip(std::string("Assets/test3.obj"));

	mCoin = new Coin("Assets/cube.txt");

	mBgTexture = new Texture2D();
	mBgTexture->LoadBMP((char*)"Assets/BgTexture.bmp");

	gameTimer = gameDuration;

	auto ptrFunc = std::bind(&GameInstance::PauseMenu, std::placeholders::_1);
	menu = glutCreateMenu(PauseMenu);

	glutAddMenuEntry("Pause", 1);
	glutAddMenuEntry("Game OVer", 2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GameInstance::DrawString(const char* text, Vector2* position, Color* color)
{
	glPushMatrix();
	glRasterPos2f(position->x, position->y);
	glColor3f(color->r, color->g, color->b);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text);
	glPopMatrix();
}

void GameInstance::DrawUI()
{
	DisableProjection();

	glBindTexture(GL_TEXTURE_2D, 0);
	std::string scoreText = "Score: " + std::to_string(mScore);
	DrawString(scoreText.c_str(), &Vector2(-0.95f, 0.9f), &Color(1.0f, 1.0f, 1.0f));

	timeText.str(std::string());
	timeText << "Time Left: " << std::setprecision(4) << gameTimer;
	std::string timeLeftText = "Time Left: " + std::to_string(gameTimer);
	DrawString(timeText.str().c_str(), &Vector2(-0.95f, 0.80f), &Color(1.0f, 1.0f, 1.0f));


	if (gameOver) {
		DrawString("Press Q to Restart", &Vector2(-0.2f, 0.0f), &Color(1.0f, 1.0f, 1.0f));
	}

	EnableProjection();
}

void GameInstance::DrawBackground()
{
	DisableProjection();
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
	EnableProjection();
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

void GameInstance::PauseMenu(int option)
{
	switch (option)
	{
	case 1:
		paused = !paused;
		break;
	case 2:
		gameOver = !gameOver;
		break;

	default:
		break;
	}
}

bool GameInstance::paused = false;
bool GameInstance::gameOver = false;


