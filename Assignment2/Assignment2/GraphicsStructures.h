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

	std::string map_Ka; //Ambient texture map
	std::string map_Kd; //Diffuse texture map
	std::string map_Ks; //Specular texture map
	std::string map_Ns; //Specular highlight map
	std::string map_d; //Alpha texture map

	std::string map_bump; //Bump map
};

struct Color
{
	float r, g, b;

	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f) {
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
	Vector3* Vertices;
	Vector3* Normals;

	GLushort* Indices;

	Vector2* TexCoords;

	int VertexCount, NormalCount, IndexCount, TexCoordCount;
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

