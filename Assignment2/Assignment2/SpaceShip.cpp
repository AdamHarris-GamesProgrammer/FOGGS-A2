#include "SpaceShip.h"

SpaceShip::SpaceShip(std::string meshFilePath) : GameObject(meshFilePath)
{
	LoadDiffuseTexture(std::string("Assets/") + mObjectMesh[1].meshMaterial.map_Kd);

	SetFriction(0.95f);
	SetRotation(Vector3(0.0f, 180.0f, 0.0f));
}

void SpaceShip::Update()
{
	GameObject::UpdatePosition();

	mRotation.x += 0.1f;
	mRotation.y += 0.2f;
	mRotation.z += 0.3f;
}

void SpaceShip::Render()
{
	for (auto& mesh : mObjectMesh) {
		if (!mesh.verticies.empty() && !mesh.indices.empty()) {
			glPushMatrix();

			glTranslatef(mPosition.x, mPosition.y, mPosition.z);

			glScalef(0.5f, 0.5f, 0.5f);

			glRotatef(mRotation.x, 1.0f, 0.0f, 0.0f);
			glRotatef(mRotation.y, 0.0f, 1.0f, 0.0f);
			glRotatef(mRotation.z, 0.0f, 0.0f, 1.0f);

			if (mTexture != nullptr) {
				glBindTexture(GL_TEXTURE_2D, mTexture->GetID());
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glBegin(GL_TRIANGLES);
			for (int i = 0; i < mesh.indices.size(); i++) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &(mesh.meshMaterial.Ka.x));
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &(mesh.meshMaterial.Kd.x));
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &(mesh.meshMaterial.Ks.x));
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mesh.meshMaterial.illum);

				glTexCoord2fv(&mesh.verticies[mesh.indices[i]].textureCoordinate.x);
				glNormal3fv(&mesh.verticies[mesh.indices[i]].normal.x);
				glVertex3fv(&mesh.verticies[mesh.indices[i]].position.x);
			}
			glEnd();

			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			glPopMatrix();
		}
	}
}

void SpaceShip::PollInput(unsigned char key, int x, int y)
{
	if (key == 'a') {
		mVelocity.x = -1.0f;
	}
	else if (key == 'd') {
		mVelocity.x = 1.0f;
	}

	if (key == 'w') {
		mVelocity.y = 1.0f;
	}
	else if (key == 's') {
		mVelocity.y = -1.0f;
	}
}

void SpaceShip::KeyUp(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'd') {
		mVelocity.x = 0.0f;
	}
	
	if (key == 'w' || key == 's') {
		mVelocity.y = 0.0f;
	}
}
