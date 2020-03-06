#include "OBJLoader.h"

OBJLoader::OBJLoader()
{

}

OBJLoader::~OBJLoader()
{
	mLoadedMeshes.clear();
}

bool OBJLoader::LoadFile(std::string path)
{
	if (path.substr(path.size() - 4, 4) != ".obj") {
		std::cout << "Invalid File Format!" << std::endl;
		return false;
	}

	std::ifstream file(path);

	if (!file.is_open()) {
		return false;
	}

	mLoadedMeshes.clear();
	mLoadedVerticies.clear();
	mLoadedIndices.clear();

	std::vector<Vector3> positions;
	std::vector<Vector3> normals;

	std::vector<Vector2> texCoords;

	std::vector<Vertex> verticies;
	std::vector<unsigned int> indices;

	std::vector < std::string> meshMaterialNames;

	bool listening = false;
	std::string meshName;

	std::string currentLine;
	while (std::getline(file, currentLine)) {
		if (Algorithm::firstToken(currentLine) == "o" || Algorithm::firstToken(currentLine) == "g" || currentLine[0] == 'g') {
			if (!listening) {
				listening = true;

				if (Algorithm::firstToken(currentLine) == "o" || Algorithm::firstToken(currentLine) == "g") {
					meshName = Algorithm::tail(currentLine);
				}
				else
				{
					meshName = "unnamed";
				}
			}
			else {
				if (!mLoadedIndices.empty() && !mLoadedVerticies.empty()) {
					Mesh tempMesh = Mesh(mLoadedVerticies, mLoadedIndices);
					tempMesh.meshName = meshName;

					mLoadedMeshes.push_back(tempMesh);

					mLoadedVerticies.clear();
					mLoadedIndices.clear();
					meshName.clear();
				}
				else
				{
					if (Algorithm::firstToken(currentLine) == "o" || Algorithm::firstToken(currentLine) == "g") {
						meshName = Algorithm::tail(currentLine);
					}
					else
					{
						meshName = "unnamed";
					}
				}
			}
		}

		//Generate a vertex position
		if (Algorithm::firstToken(currentLine) == "v") {
			std::vector<std::string> spos;
			Vector3 vpos;
			Algorithm::Split(Algorithm::tail(currentLine), spos, " ");

			vpos.x = std::stof(spos[0]);
			vpos.y = std::stof(spos[1]);
			vpos.z = std::stof(spos[2]);

			positions.push_back(vpos);
		}

		//Generate a vertex texture coordinate
		if (Algorithm::firstToken(currentLine) == "vt") {
			std::vector<std::string> stex;
			Vector2 vtex;
			Algorithm::Split(currentLine, stex, " ");

			vtex.x = std::stof(stex[0]);
			vtex.y = std::stof(stex[1]);

			texCoords.push_back(vtex);
		}

		//Generates vertex normal coordinates
		if (Algorithm::firstToken(currentLine) == "vn") {
			std::vector<std::string> snor;
			Vector3 vnor;
			Algorithm::Split(currentLine, snor, " ");

			vnor.x = std::stof(snor[0]);
			vnor.y = std::stof(snor[1]);
			vnor.z = std::stof(snor[2]);

			normals.push_back(vnor);
		}

		//Generates a face from verticies and indices
		if (Algorithm::firstToken(currentLine) == "f") {
			std::vector<Vertex> vVerts;
			GenerateVerticiesFromRawOBJ(vVerts, positions, texCoords, normals, currentLine);

			for (int i = 0; i < int(vVerts.size()); i++) {
				verticies.push_back(vVerts[i]);

				mLoadedVerticies.push_back(vVerts[i]);
			}

			std::vector<unsigned int> inIndices;
			VertexTriangulation(inIndices, vVerts);

			//Add Indices
			for (int i = 0; i < int(inIndices.size()); i++) {
				unsigned int indicesNum = (unsigned int)((mLoadedVerticies.size()) - vVerts.size()) + inIndices[i];
				mLoadedIndices.push_back(indicesNum);
			}
		}

		//Gets the material name (if applicable)
		if (Algorithm::firstToken(currentLine) == "usemtl") {
			meshMaterialNames.push_back(Algorithm::tail(currentLine));

			if (!indices.empty() && !verticies.empty()) {
				Mesh tempMesh = Mesh(verticies, indices);
				tempMesh.meshName = meshName;
				int i = 2;
				while (1) {
					tempMesh.meshName = meshName + "_" + std::to_string(i);

					for (auto& m : mLoadedMeshes) {
						if (m.meshName == tempMesh.meshName) {
							continue;
						}
					}
					break;
				}

				mLoadedMeshes.push_back(tempMesh);

				//Cleanup 
				verticies.clear();
				indices.clear();
			}
		}

		//Load Materials
		if (Algorithm::firstToken(currentLine) == "mtlib") {
			std::vector <std::string> temp;
			Algorithm::Split(path, temp, "/");
			
			std::string pathToMaterial = "";

			if (temp.size() != 1) {
				for (int i = 0; i < temp.size() - 1; i++) {
					pathToMaterial += temp[i] + "/";
				}
			}

			pathToMaterial += Algorithm::tail(currentLine);

			LoadMaterials(pathToMaterial);
		}

		//Deals with remaining vertices and indices
		if (!indices.empty() && !verticies.empty()) {
			Mesh tempMesh = Mesh(verticies, indices);
			tempMesh.meshName = meshName;

			mLoadedMeshes.push_back(tempMesh);
		}

		file.close();

		//Set materials for each mesh
		for (int i = 0; i < meshMaterialNames.size(); i++) {
			std::string matname = meshMaterialNames[i];

			for (int j = 0; j < mLoadedMaterial.size(); j++) {
				if (mLoadedMaterial[j].name == matname) {
					mLoadedMeshes[i].meshMaterial = mLoadedMaterial[j];
					break;
				}
			}
		}

		if (mLoadedMeshes.empty() && mLoadedVerticies.empty() && mLoadedIndices.empty()) {
			return false;
		}
		else
		{
			return true;
		}
	}
}

void OBJLoader::GenerateVerticiesFromRawOBJ(std::vector<Vertex>& outVerts, const std::vector<Vector3>& inPositions, const std::vector<Vector2>& inTexCoords, const std::vector<Vector3>& inNormals, std::string& inLine)
{
	std::vector<std::string> sface, svert;
	Vertex vVert;
	Algorithm::Split(inLine, sface, " ");

	bool noNormal = false;

	//for every vertex do this
	for (int i = 0; i < int(sface.size()); i++) {

	}
}

void OBJLoader::VertexTriangulation(std::vector<unsigned int>& outIndices, const std::vector<Vertex>& inVerts)
{

}

bool OBJLoader::LoadMaterials(std::string path)
{

}
