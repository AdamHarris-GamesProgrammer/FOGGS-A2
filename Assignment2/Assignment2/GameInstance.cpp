#include "GameInstance.h"
#include "GlutCallbacks.h"
#include "Constants.h"

#include <iostream>
#include "GraphicsStructures.h"

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

	glPushMatrix();

	glRotatef(rotation, 0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);

	for (auto& object : loader.mLoadedMeshes) {
		for (int i = 0; i < object.indices.size(); i++) {
			glTexCoord2fv(&object.verticies[object.indices[i]].textureCoordinate.x);
			glNormal3fv(&object.verticies[object.indices[i]].normal.x);
			glVertex3fv(&object.verticies[object.indices[i]].position.x);
		}
	}

	glEnd();

	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void GameInstance::Update()
{
	glLoadIdentity();

	rotation += 0.1f;

	glTranslatef(0.0f, 0.0f, -5.0f);

	glutPostRedisplay();
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

	glMatrixMode(GL_PROJECTION);

	glViewport(0, 0, 800, 800);
	gluPerspective(45, 1, 1, 1000);


	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMAL_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

void GameInstance::InitObjects()
{
	mCamera = new Camera();
	mCamera->eye.z = 1.0f;
	mCamera->up.y = 1.0f;


	bool cubeLoad = loader.LoadFile("box_stack.obj");

	for (auto& object : loader.mLoadedMeshes) {
		std::cout << object.meshName << " is initialized " << std::endl;
	}
}
