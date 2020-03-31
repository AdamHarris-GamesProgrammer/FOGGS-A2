#include "Texture2D.h"
#include <iostream>
#include <fstream>
#include "BMP.h"

Texture2D::Texture2D()
{

}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &mID);
}

bool Texture2D::Load(const char* path, int width, int height)
{
	char* tempTexturedData;
	int fileSize;
	std::ifstream inFile;

	mWidth = width;
	mHeight = height;
	inFile.open(path, std::ios::binary);

	if (!inFile.good()) {
		std::cerr << "Can't open texture file: " << path << std::endl;
		return false;
	}

	inFile.seekg(0, std::ios::end);

	fileSize = (int)inFile.tellg();
	tempTexturedData = new char[fileSize];
	inFile.seekg(0, std::ios::beg);
	inFile.read(tempTexturedData, fileSize);
	inFile.close();

	std::cout << path << " loaded." << std::endl;

	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTexturedData);

	delete[] tempTexturedData;
	return true;
}

bool Texture2D::LoadBMP(const char* path, GLenum type)
{
	BMP tempTexturedData(path);

	mWidth = tempTexturedData.infoHeader.width;
	mHeight = tempTexturedData.infoHeader.height;

	tempTexturedData.ConvertToRGB();
	std::cout << path << " is loaded\n";

	glGenTextures(1, &mID);
	glBindTexture(type, mID);
	glTexImage2D(type, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTexturedData.rgbData.data());

	return true;
}

bool Texture2D::LoadBMP(const char* path)
{
	BMP tempTexturedData(path);

	mWidth = tempTexturedData.infoHeader.width;
	mHeight = tempTexturedData.infoHeader.height;

	tempTexturedData.ConvertToRGB();
	std::cout << path << " is loaded\n";

	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tempTexturedData.rgbData.data());

	return true;
}