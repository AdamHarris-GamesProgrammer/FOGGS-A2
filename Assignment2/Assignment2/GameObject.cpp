#include "GameObject.h"

GameObject::GameObject(std::string meshFilePath)
{
	mMeshLoader = new OBJLoader();
	mMeshLoader->LoadFile(meshFilePath);

	mObjectMesh.reserve(mMeshLoader->mLoadedMeshes.size());
	
	for (auto mesh : mMeshLoader->mLoadedMeshes) {
		mObjectMesh.emplace_back(mesh);
	}
}

GameObject::~GameObject()
{
	
}

void GameObject::Render()
{
	for (auto& mesh : mObjectMesh) {
		if (!mesh.verticies.empty() && !mesh.indices.empty()) {
			glPushMatrix();

			glTranslatef(mPosition.x, mPosition.y, mPosition.z);

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

void GameObject::Update()
{
	UpdatePosition();
}

void GameObject::UpdatePosition()
{
	mPosition.x += mVelocity.x * mFriction;
	mPosition.y += mVelocity.y * mFriction;
	mPosition.z += mVelocity.z * mFriction;
	//printf("Velocity: (%f,%f,%f)\n", mVelocity.x, mVelocity.y, mVelocity.z);
}

void GameObject::PollInput(unsigned char key, int x, int y)
{

}

void GameObject::LoadDiffuseTexture(std::string filePath)
{
	mTexture = new Texture2D();
	//TODO: Add bmp extension check
	mTexture->LoadBMP(filePath.c_str());
}

