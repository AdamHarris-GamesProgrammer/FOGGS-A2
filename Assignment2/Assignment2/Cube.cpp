#include "Cube.h"
#include "Cube.h"

#include <ctime>
#include <fstream>
#include <iostream>

Cube::Cube(TextMesh* mesh, Texture2D* texture, float xPos, float yPos, float zPos)
{
	mMesh = mesh;
	mTexture = texture;

	mPosition.x = xPos;
	mPosition.y = yPos;
	mPosition.z = zPos;

	mMaterial = new Material();
	mMaterial->Ka.x = (rand() % 10) / 100.0f; mMaterial->Ka.y = (rand() % 10) / 100.0f; mMaterial->Ka.z = (rand() % 10) / 100.0f;
	mMaterial->Kd.x = 0.05f; mMaterial->Kd.y = 0.05f; mMaterial->Kd.z = 0.05f;
	mMaterial->Ks.x = 1.0f; mMaterial->Ks.y = 1.0f; mMaterial->Ks.z = 1.0f; 
	mMaterial->illum = 100.0f;
}

Cube::~Cube()
{

}

void Cube::Draw()
{
	if (mMesh->Vertices != nullptr && mMesh->Normals != nullptr && mMesh->Indices != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, mTexture->GetID());

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glPushMatrix();

		glTranslatef(mPosition.x, mPosition.y, mPosition.z);

		glRotatef(mRotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(mRotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(mRotation.z, 0.0f, 0.0f, 1.0f);

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < mMesh->IndexCount; i++)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, &(mMaterial->Ka.x));
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &(mMaterial->Kd.x));
			glMaterialfv(GL_FRONT, GL_SPECULAR, &(mMaterial->Ks.x));
			glMaterialf(GL_FRONT, GL_SHININESS, mMaterial->illum);

			glTexCoord2fv(&mMesh->TexCoords[mMesh->Indices[i]].x);
			glNormal3fv(&mMesh->Normals[mMesh->Indices[i]].x);
			glVertex3fv(&mMesh->Vertices[mMesh->Indices[i]].x);
		}
		glEnd();

		glPopMatrix();

	}

}

void Cube::Update()
{
	if (mRotation.x > 360.0f) {
		mRotation.x = 0.0f;
	}
	if (mRotation.y > 360.0f) {
		mRotation.y = 0.0f;
	}
	if (mRotation.z > 360.0f) {
		mRotation.z = 0.0f;
	}

	srand(time(0));
	mRotation.x += (rand() % 10) / 10.0f;
	mRotation.y += (rand() % 10) / 10.0f;
	mRotation.z += (rand() % 10) / 10.0f;
}