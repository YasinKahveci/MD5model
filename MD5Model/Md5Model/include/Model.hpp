#pragma once
#include <Mesh.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "Helpers.hpp"
#include "Joint.hpp"
#include "Mesh.hpp"
#include "Vertex.hpp"
#include "MeshAnimation.hpp"
class ModelMD5
{
public:
	ModelMD5();

	bool loadModel(const std::string& fileName);
	bool loadModelAnim(const std::string& fileName);
	void Draw(float DeltaTime);
	glm::vec3 getLookAt();
	void setAnimActive();
	void ClearAnim();
	float getMaxSize();
private:
	unsigned int m_MD5Version;
	unsigned int m_numJoints;
	unsigned int m_numMeshes;
	std::vector<Joint> m_Joints;
	std::vector<Mesh> m_Meshs;
	MeshAnimation m_Anim;
	void ComputePositions(Mesh& mesh);
	void ComputePositions(Mesh& mesh, const std::vector<Joint>& skel);
	void ComputeNormals(Mesh& mesh);
	void Update(float DeltaTime);
	bool ModelRestart;
	void ClearModel();
};