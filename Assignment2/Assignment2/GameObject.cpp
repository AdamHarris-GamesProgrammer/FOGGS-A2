#include "GameObject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	delete mTexture;
}

void GameObject::Render()
{
	
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

	std::string fileType = filePath.substr(filePath.size() - 4, filePath.size());

	if (fileType == ".bmp") {
		mTexture->LoadBMP(filePath.c_str());
	}
	else if (fileType == ".raw") {
		mTexture->Load(filePath.c_str(), 512,512);
	}
}

void GameObject::LoadNormalTexture(std::string filePath)
{
	mTexture = new Texture2D();

	std::string fileType = filePath.substr(filePath.size() - 4, filePath.size());

	if (fileType == ".bmp") {
		mTexture->LoadBMP(filePath.c_str());
	}
	else if (fileType == ".raw") {
		mTexture->Load(filePath.c_str(), 512, 512);
	}
}

