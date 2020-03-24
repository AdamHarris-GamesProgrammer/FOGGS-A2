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

void TextGameObject::Render()
{
	if (mMesh->Vertices != nullptr && mMesh->Normals != nullptr && mMesh->Indices != nullptr)
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
