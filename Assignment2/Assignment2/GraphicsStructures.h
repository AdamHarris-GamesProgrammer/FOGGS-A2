#pragma once
#include "MathStructures.h"

#include <string>
#include <iostream>
#include <vector>

#include "GLIncludes.h"

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector2 textureCoordinate;
};

struct Material
{
	Material() {
		name;
		Ns = 0.0f;
		Ni = 0.0f;
		d = 0.0f;
		illum = 0.0f;
	}

	std::string name;

	Vector3 Ka; //Ambient Colour
	Vector3 Kd; //DiffuseColour
	Vector3 Ks; //Specular Component

	float Ns; //Specular exponent
	float Ni; //optical density
	float d; //dissolve
	float illum; //illumination

	std::string mapKa; //Ambient texture map
	std::string mapKd; //Diffuse texture map
	std::string mapKs; //Specular texture map
	std::string mapNs; //Specular highlight map
	std::string mapD; //Alpha texture map

	std::string mapBump; //Bump map
};

struct Colour
{
	float r, g, b;

	Colour(float red = 0.0f, float green = 0.0f, float blue = 0.0f) {
		r = red;
		g = green;
		b = blue;
	}
};

struct Mesh {
	Mesh() {}

	Mesh(std::vector<Vertex>& inVerticies, std::vector<unsigned int>& inIndices) : verticies(inVerticies), indices(inIndices) {}

	std::string meshName;

	std::vector<Vertex>verticies;
	std::vector<unsigned int>indices;

	Material meshMaterial;
};

struct TextMesh {
	Vector3* vertices;
	Vector3* normals;

	GLushort* indices;

	Vector2* texCoords;

	int vertexCount, normalCount, indexCount, texCoordCount;
};

struct Light {
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;

	Light() {
		ambient = Vector3(0.8f, 0.8f, 0.8f);
		diffuse = Vector3(0.8f, 0.8f, 0.8f);
		specular = Vector3(0.8f, 0.8f, 0.8f);
	}
};

