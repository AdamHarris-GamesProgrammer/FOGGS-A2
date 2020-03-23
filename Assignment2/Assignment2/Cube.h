#pragma once

#include "GLIncludes.h"

#include "MathStructures.h"
#include "GraphicsStructures.h"
#include "TextMeshLoader.h"
#include "Texture2D.h"

class Cube
{
public:
	Cube(TextMesh* mesh, Texture2D* texutre, float xPos, float yPos, float zPos);
	~Cube();

	void Draw();
	void Update();

	Vector3 GetRotation() { return mRotation; }
	void SetRotation(Vector3 newRotation) { mRotation = newRotation; }

private:
	Material* mMaterial;

	TextMesh* mMesh;
	Texture2D* mTexture;

	Vector3 mRotation;
	Vector3 mPosition;
};

