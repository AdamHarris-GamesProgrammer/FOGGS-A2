#pragma once
#include "GraphicsStructures.h"

#include "GLIncludes.h"

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	bool Load(const char* path, int width, int height);
	bool LoadBMP(const char* path);

	GLuint GetID() const { return mID; }

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }

private:
	GLuint mID;

	int mWidth, mHeight;
};

