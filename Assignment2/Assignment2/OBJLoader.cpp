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
		int vType;

		Algorithm::Split(sface[i], svert, "/");

		//Checks for just position (v1)
		if (svert.size() == 1) {
			vType = 1;
		}

		//Checks for position and texture (v1/vt1)
		if (svert.size() == 2) {
			vType = 2;
		}

		//Checks for position, texture and normal (v1,vt1,vn1) or if just position and normal
		if (svert.size() == 3) {
			if (svert[1] != "") {
				//position texture and normal
				vType = 4;
			}
			else
			{
				//position and normal
				vType = 3;
			}
		}

		switch (vType)
		{
		case 1: //Just position
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Vector2();
			noNormal = true;
			outVerts.push_back(vVert);
			break;
		case 2: //Position and texture
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Algorithm::GetElement(inTexCoords, svert[1]);
			noNormal = true;
			outVerts.push_back(vVert);
			break;
		case 3: //Position and normals
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Vector2();
			vVert.normal = Algorithm::GetElement(inNormals, svert[2]);
			outVerts.push_back(vVert);
		case 4: //Position, texture and normals
			vVert.position = Algorithm::GetElement(inPositions, svert[0]);
			vVert.textureCoordinate = Algorithm::GetElement(inTexCoords, svert[1]);
			vVert.normal = Algorithm::GetElement(inNormals, svert[2]);
			outVerts.push_back(vVert);
			break;
		default:
			break;
		}


		//Generates a normal if no normal is found
		if (noNormal) {
			Vector3 A = outVerts[0].position - outVerts[1].position;
			Vector3 B = outVerts[2].position - outVerts[1].position;

			Vector3 normal = Math::CrossProduct(A, B);

			for (int i = 0; i < int(outVerts.size()); i++) {
				outVerts[i].normal = normal;
			}
		}
	}
}

void OBJLoader::VertexTriangulation(std::vector<unsigned int>& outIndices, const std::vector<Vertex>& inVerts)
{
	if (inVerts.size() < 3) {
		return;
	}

	if (inVerts.size() == 3) {
		outIndices.push_back(0);
		outIndices.push_back(1);
		outIndices.push_back(2);
		return;
	}

	std::vector<Vertex> tVerts = inVerts;

	while (true)
	{
		for (int i = 0; i < int(tVerts.size()); i++) {
			Vertex pPrev;
			if (i == 0) {
				pPrev = tVerts[tVerts.size() - 1];
			}
			else
			{
				pPrev = tVerts[i - 1];
			}

			Vertex pCur = tVerts[i];

			Vertex pNext;
			if (i == tVerts.size() - 1) {
				pNext = tVerts[0];
			}
			else
			{
				pNext = tVerts[i + 1];
			}

			if (tVerts.size() == 3) {
				for (int j = 0; j < int(tVerts.size()); j++) {
					if (inVerts[j].position == pCur.position) {
						outIndices.push_back(j);
					}
					if (inVerts[j].position == pPrev.position) {
						outIndices.push_back(j);
					}
					if (inVerts[j].position == pPrev.position) {
						outIndices.push_back(j);
					}
				}
				tVerts.clear();
				break;
			}

			if (tVerts.size() == 4) {
				for (int j = 0; j < int(tVerts.size()); j++) {
					if (inVerts[j].position == pCur.position) {
						outIndices.push_back(j);
					}
					if (inVerts[j].position == pPrev.position) {
						outIndices.push_back(j);
					}
					if (inVerts[j].position == pPrev.position) {
						outIndices.push_back(j);
					}
				}

				Vector3 tempVec;
				for (int j = 0; int(tVerts.size()); j++) {
					if (tVerts[j].position != pCur.position && tVerts[j].position != pPrev.position && tVerts[j].position != pNext.position) {
						tempVec = tVerts[j].position;
						break;
					}
				}

				for (int j = 0; j < int(inVerts.size()); j++) {
					if (inVerts[j].position == pPrev.position) {
						outIndices.push_back(j);
					}
					if (inVerts[j].position == pNext.position) {
						outIndices.push_back(j);
					}
					if (inVerts[j].position == tempVec) {
						outIndices.push_back(j);
					}
				}

				tVerts.clear();
				break;
			}

			//If vertex is not in interior vertex
			float angle = Math::AngleBetweenVectors(pPrev.position - pCur.position, pNext.position - pCur.position);
			if (angle <= 0 && angle >= 180) {
				continue;
			}

			bool inTriangle = false;
			for (int j = 0; int(tVerts.size()); j++) {
				if (Algorithm::InTriangle(inVerts[j].position, pPrev.position, pCur.position, pNext.position)
					&& inVerts[j].position != pPrev.position
					&& inVerts[j].position != pCur.position
					&& inVerts[j].position != pNext.position) {
					inTriangle = true;
					break;
				}
			}

			if (inTriangle) {
				continue;
			}

			//Create a triangle from pCur, pPrev and pNext
			for (int j = 0; j < int(inVerts.size()); j++) {
				if (inVerts[j].position == pCur.position) {
					outIndices.push_back(j);
				}
				if (inVerts[j].position == pPrev.position) {
					outIndices.push_back(j);
				}
				if (inVerts[j].position == pNext.position) {
					outIndices.push_back(j);
				}
			}

			//delete pCur 
			for (int j = 0; j < int(tVerts.size()); j++) {
				if (tVerts[j].position == pCur.position) {
					tVerts.erase(tVerts.begin() + j);
					break;
				}
			}

			//reset i to the start
			i = -1;
		}

		//if no triangle were created
		if (outIndices.size() == 0) {
			break;
		}

		//if no more vertices
		if (tVerts.size() == 0) {
			break;
		}
	}
}

bool OBJLoader::LoadMaterials(std::string path)
{
	if (path.substr(path.size() - 4, path.size()) != ".mtl") {
		std::cout << "Wrong file format" << std::endl;
		return false;
	}

	std::ifstream file(path);

	if (!file.is_open()) {
		return false;
	}

	Material tempMaterial;

	bool listening = false;

	std::string currentLine;

	while (std::getline(file, currentLine)) {
		//Finds the material name
		if (Algorithm::firstToken(currentLine) == "newmtl") {
			if (!listening) {
				listening = true;

				if (currentLine.size() > 7) {
					tempMaterial.name = Algorithm::tail(currentLine);
				}
				else
				{
					tempMaterial.name = "none";
				}
			}
		}

		//Ambient Colour
		if (Algorithm::firstToken(currentLine) == "Ka") {
			std::vector<std::string> temp;
			Algorithm::Split(Algorithm::tail(currentLine), temp, " ");

			if (temp.size() != 3) {
				continue;
			}

			tempMaterial.Ka.x = std::stof(temp[0]);
			tempMaterial.Ka.y = std::stof(temp[1]);
			tempMaterial.Ka.z = std::stof(temp[2]);
		}

		//Diffuse Colour
		if (Algorithm::firstToken(currentLine) == "Kd") {
			std::vector<std::string> temp;
			Algorithm::Split(Algorithm::tail(currentLine), temp, " ");

			if (temp.size() != 3) {
				continue;
			}

			tempMaterial.Kd.x = std::stof(temp[0]);
			tempMaterial.Kd.y = std::stof(temp[1]);
			tempMaterial.Kd.z = std::stof(temp[2]);
		}

		//Specular colour
		if (Algorithm::firstToken(currentLine) == "Ks") {
			std::vector<std::string> temp;
			Algorithm::Split(Algorithm::tail(currentLine), temp, " ");

			if (temp.size() != 3) {
				continue;
			}

			tempMaterial.Ks.x = std::stof(temp[0]);
			tempMaterial.Ks.y = std::stof(temp[1]);
			tempMaterial.Ks.z = std::stof(temp[2]);
		}

		//Specular exponent
		if (Algorithm::firstToken(currentLine) == "Ns") {
			tempMaterial.Ns = std::stof(Algorithm::tail(currentLine));
		}

		if (Algorithm::firstToken(currentLine) == "Ni") {
			tempMaterial.Ni = std::stof(Algorithm::tail(currentLine));
		}

		if (Algorithm::firstToken(currentLine) == "d") {
			tempMaterial.d = std::stof(Algorithm::tail(currentLine));
		}

		if (Algorithm::firstToken(currentLine) == "illum") {
			tempMaterial.illum = std::stof(Algorithm::tail(currentLine));
		}
		
		if (Algorithm::firstToken(currentLine) == "map_Ka") {
			tempMaterial.map_Ka = std::stof(Algorithm::tail(currentLine));
		}

		if (Algorithm::firstToken(currentLine) == "map_Kd") {
			tempMaterial.map_Kd = std::stof(Algorithm::tail(currentLine));
		}

		if (Algorithm::firstToken(currentLine) == "map_Ks") {
			tempMaterial.map_Ks = std::stof(Algorithm::tail(currentLine));
		}

		if (Algorithm::firstToken(currentLine) == "map_Ns") {
			tempMaterial.map_Ns = std::stof(Algorithm::tail(currentLine));
		}

		if (Algorithm::firstToken(currentLine) == "map_D") {
			tempMaterial.map_d = std::stof(Algorithm::tail(currentLine));
		}

		//Bump Map
		if (Algorithm::firstToken(currentLine) == "map_Bump" || Algorithm::firstToken(currentLine) == "map_bump" || Algorithm::firstToken(currentLine) == "bump") {
			tempMaterial.map_bump = Algorithm::tail(currentLine);
		}

	}

	mLoadedMaterial.push_back(tempMaterial);

	if (mLoadedMaterial.empty()) {
		return false;
	}
	else
	{
		return true;
	}
}
