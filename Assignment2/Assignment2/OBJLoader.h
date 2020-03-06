#pragma once

#include "MathStructures.h"
#include "GraphicsStructures.h"

#include <string>
#include <fstream>

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	bool LoadFile(std::string path);

	std::vector<Mesh> mLoadedMeshes;
	std::vector<Vertex> mLoadedVerticies;
	std::vector<unsigned int> mLoadedIndices;
	std::vector<Material> mLoadedMaterial;

private:
	void GenerateVerticiesFromRawOBJ(std::vector<Vertex>& outVerts, const std::vector<Vector3>& inPositions, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::string& inLine);
	void VertexTriangulation(std::vector<unsigned int>& outIndices, const std::vector<Vertex>& inVerts);

	bool LoadMaterials(std::string path);
};

