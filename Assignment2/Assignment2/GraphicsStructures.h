#pragma once
#include "MathStructures.h"

#include <string>
#include <iostream>
#include <vector>

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector2 textureCoordinate;
};

struct Material
{
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

struct Mesh {
	Mesh() {}

	Mesh(std::vector<Vertex>& inVerticies, std::vector<unsigned int>& inIndices) : verticies(inVerticies), indices(inIndices){}

	std::string meshName;

	std::vector<Vertex>verticies;
	std::vector<unsigned int>indices;

	Material meshMaterial;
};