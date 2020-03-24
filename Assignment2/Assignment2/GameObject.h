#pragma once

#include "GLIncludes.h"

#include "MathStructures.h"
#include "GraphicsStructures.h"

#include "OBJLoader.h"
#include "Texture2D.h"

#include <string>
#include <iostream>
#include <vector>
#include "TextMeshLoader.h"

class GameObject
{
public:
	~GameObject();

	GameObject();


	virtual void Render();
	virtual void Update();

	virtual void UpdatePosition();

	virtual void PollInput(unsigned char key, int x, int y);

	void LoadDiffuseTexture(std::string filePath);
	
	Vector3 GetPosition() const { return mPosition; }
	Vector3 GetVelocity() const { return mVelocity; }
	Vector3 GetRotation() const { return mRotation; }
	Vector3 GetScale() const { return mScale; }
	float GetFriction() const { return mFriction; }

	void SetPosition(Vector3 newPosition) { mPosition = newPosition; }
	void SetVelocity(Vector3 newVelocity) { mVelocity = newVelocity; }
	void SetRotation(Vector3 newRotation) { mRotation = newRotation; }
	void SetScale(Vector3 newScale) { mScale = newScale; }
	void SetFriction(float newFriction) { mFriction = newFriction; }

private:

protected:
	Vector3 mPosition;
	Vector3 mVelocity;
	Vector3 mRotation;
	Vector3 mScale;

	Texture2D* mTexture;

	float mFriction = 0.0f;
};

