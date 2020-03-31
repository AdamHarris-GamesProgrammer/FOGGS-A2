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
	srand(time(NULL));

	InitOpenGL(argc, argv);
	InitObjects();

	glutMainLoop();
}

GameInstance::~GameInstance()
{
	glutDestroyMenu(menu);

	delete mSpaceShip;
	delete mCoin;

	delete mCamera;
	delete mLight;

	delete mBgTexture;
	delete mCollisionsInstance;
}

void GameInstance::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Demonstration of the painters algorithm (Back to front drawing)
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

	//Game Logic
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

	//OpgnGL related update calls
	glLoadIdentity();

	mCamera->Update();

	glLightfv(GL_LIGHT0, GL_AMBIENT, &(mLight->ambient.x));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, &(mLight->diffuse.x));
	glLightfv(GL_LIGHT0, GL_SPECULAR, &(mLight->specular.x));
	

	glutPostRedisplay();
}

void GameInstance::Keyboard(unsigned char key, int x, int y)
{
	//if game is not over poll player input
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

	//Pauses game
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
	//Handles the first mouse position, this stops the camera from using the initial mouse position for calculations
	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	//Calculates the difference between last frames mouse position and this frames mouse position
	float xOffset = x - lastX;
	float yOffset = lastY - y; //Opposite as coordinate system is from top left
	lastX = x;
	lastY = y;

	//if follow mouse is true then process the input based off the x and y offset variables
	if (followMouse) {
		mCamera->ProcessInput(xOffset, yOffset);
	}

}

void GameInstance::InitOpenGL(int argc, char* argv[])
{
	GLUTCallback::Init(this);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT); //800x800
	glutCreateWindow("FOGGS Assignment 2 - Adam Harris | Star Hopper");

	//Sets function callbacks
	glutKeyboardFunc(GLUTCallback::Keyboard);
	glutKeyboardUpFunc(GLUTCallback::KeyboardUp);
	glutPassiveMotionFunc(GLUTCallback::ActiveMouseMotion);
	glutTimerFunc(REFRESH_RATE, GLUTCallback::Timer, REFRESH_RATE);
	glutDisplayFunc(GLUTCallback::Display);

	glMatrixMode(GL_PROJECTION);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	gluPerspective(45, 1, 1, 40);


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

	
	mSpaceShip = new SpaceShip(std::string("Assets/spaceship.obj"));

	//Coin game object is based off the cube.txt model file
	mCoin = new Coin("Assets/cube.txt");

	mBgTexture = new Texture2D();
	mBgTexture->LoadBMP((char*)"Assets/BgTexture.bmp");

	//sets the timer to the game length
	gameTimer = gameDuration;

	//Sets up glut menu for the pause and game over
	menu = glutCreateMenu(PauseMenu); //Pause menu needs to be static in order to pass the function in as a callback

	//Adds "Pause" and "Game Over" to the menu as options with there id's as 1 and 2
	glutAddMenuEntry("Pause", 1);
	glutAddMenuEntry("Game OVer", 2);

	//Attaches this menu to the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GameInstance::DrawString(const char* text, Vector2* position, Colour* color)
{
	glPushMatrix();
	glRasterPos2f(position->x, position->y); //Sets the texts position in relation to NDC (Normalized Device Coordinates [-1 to 1])
	glColor3f(color->r, color->g, color->b); //Sets the colour of the text
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)text); //Sets the font and the text
	glPopMatrix();
}

void GameInstance::DrawUI()
{
	DisableProjection(); //Disables projection viewing allowing the UI to be drawn in relation to the screen not the camera

	glBindTexture(GL_TEXTURE_2D, 0); //binds a blank texture
	std::string scoreText = "Score: " + std::to_string(mScore);
	DrawString(scoreText.c_str(), &Vector2(-0.95f, 0.9f), &Colour(1.0f, 1.0f, 1.0f));

	timeText.str(std::string());
	timeText << "Time Left: " << std::setprecision(4) << gameTimer; //std::setprecision(4) means cap gameTimer to four significan figures
	std::string timeLeftText = "Time Left: " + std::to_string(gameTimer);
	DrawString(timeText.str().c_str(), &Vector2(-0.95f, 0.80f), &Colour(1.0f, 1.0f, 1.0f));


	if (gameOver) {
		DrawString("Press Q to Restart", &Vector2(-0.2f, 0.0f), &Colour(1.0f, 1.0f, 1.0f));
	}

	EnableProjection(); //Re enables the projection for 3D viewing 
}

void GameInstance::DrawBackground()
{
	//This method disables 3d viewing draws a texture on a 2D quad going across the whole screen and then re enables 3d viewing
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
	//This switch statement tests the option that the player clicks on in the menu
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

//these variables have to be static to work in the pause menu function which needs to be static to be used as a callback in the create menu method
bool GameInstance::paused = false;
bool GameInstance::gameOver = false;


