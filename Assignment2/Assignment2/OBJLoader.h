#pragma once

#include "MathStructures.h"
#include "GraphicsStructures.h"

#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>

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
	void GenerateVerticiesFromRawOBJ(std::vector<Vertex>& outVerts, const std::vector<Vector3>& inPositions, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::string inLine);
	void VertexTriangulation(std::vector<unsigned int>& outIndices, const std::vector<Vertex>& inVerts);

	bool LoadMaterials(std::string path);

	void LoadVertices(std::vector<Vector3>& inPositions, std::string& currentLine);
	void LoadVertexTextures(std::vector<Vector2>& inTexCoords, std::string& currentLine);
	void LoadVertexNormals(std::vector<Vector3>& inNormals, std::string& currentLine);
	void LoadFaces(std::vector<Vertex>& inVertices,std::vector<unsigned int>& inIndices, std::vector<Vector3>& inPositions, std::vector<Vector2>& inTexCoords, std::vector<Vector3>& inNormals, std::string& inCurrentLine);

	void GetMaterialName(Mesh& inTempMesh,std::string& inMeshName, std::vector<std::string>& inMeshMaterialNames, std::vector<Vertex>& inVertices, std::vector<unsigned int>& inIndices, std::string& inCurrentLine);
	std::string GetMaterialFilePath(std::string& inPath,std::string& inCurrentLine);

	void LoadColourAmbient(Material& inMaterial, std::string& inCurrentLine);
	void LoadColourDiffuse(Material& inMaterial, std::string& inCurrentLine);
	void LoadColourSpecular(Material& inMaterial, std::string& inCurrentLine);
};

