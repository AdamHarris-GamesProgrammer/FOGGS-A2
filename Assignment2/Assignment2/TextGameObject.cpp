#include "TextGameObject.h"

TextGameObject::TextGameObject(std::string meshFilePath)
{
	mMesh = TextMeshLoader::Load(meshFilePath.c_str());

	mMaterial = new Material();
	mMaterial->Ka.x = 1.0f; mMaterial->Ka.y = 1.0f; mMaterial->Ka.z = 1.0f;
	mMaterial->Kd.x = 0.05f; mMaterial->Kd.y = 0.05f; mMaterial->Kd.z = 0.05f;
	mMaterial->Ks.x = 1.0f; mMaterial->Ks.y = 1.0f; mMaterial->Ks.z = 1.0f;
	mMaterial->illum = 100.0f;
}

TextGameObject::~TextGameObject()
{
	delete mMesh;
	delete mMaterial;
}

void TextGameObject::Render()
{
	if (mMesh->vertices != nullptr && mMesh->normals != nullptr && mMesh->indices != nullptr)
	{
		if (mTexture != nullptr) {
			glBindTexture(GL_TEXTURE_2D, mTexture->GetID());
		}

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glPushMatrix();

		glTranslatef(mPosition.x, mPosition.y, mPosition.z);

		glRotatef(mRotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(mRotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(mRotation.z, 0.0f, 0.0f, 1.0f);

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < mMesh->indexCount; i++)
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, &(mMaterial->Ka.x));
			glMaterialfv(GL_FRONT, GL_DIFFUSE, &(mMaterial->Kd.x));
			glMaterialfv(GL_FRONT, GL_SPECULAR, &(mMaterial->Ks.x));
			glMaterialf(GL_FRONT, GL_SHININESS, mMaterial->illum);

			glTexCoord2fv(&mMesh->texCoords[mMesh->indices[i]].x);
			glNormal3fv(&mMesh->normals[mMesh->indices[i]].x);
			glVertex3fv(&mMesh->vertices[mMesh->indices[i]].x);
		}
		glEnd();

		glPopMatrix();

	}
}
