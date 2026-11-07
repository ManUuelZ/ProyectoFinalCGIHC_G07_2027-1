#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

#include "Mesh_tn.h"
#include "Texture.h"

class Model
{
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	// Caja que envuelve al modelo (la calcula LoadModel y la usa ValleHelado::colocar)
	aiVector3D minimo, maximo;
	bool cargado() const { return !MeshList.empty(); }

	~Model();

private:
	void LoadNode(aiNode* node, const aiScene* scene); //assimp
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	std::vector<MeshModel*> MeshList;
	std::vector<Texture*> TextureList;
	std::vector<unsigned int> meshTotex;
	std::string directorioModelo;
};