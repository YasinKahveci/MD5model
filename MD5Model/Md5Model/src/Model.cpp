#include "Model.hpp"

ModelMD5::ModelMD5()
{
	
}

bool ModelMD5::loadModel(const std::string& fileName)
{
	ClearModel();
	std::ifstream file(fileName);

	if (file.fail())
	{
		std::cout << fileName + " couldn't open" << std::endl;
		return false;
	}

	std::string Key;
	std::string Skip;
	m_Meshs.clear();
	m_Joints.clear();
	ModelRestart = false;
	int LengthofFile = GetFileLength(file);
	file >> Key;
	while (!file.eof())
	{
		if (Key == "MD5Version")
		{
			file >> m_MD5Version;
			if (m_MD5Version != 10)
			{
				return false;
			}
		}
		else if (Key == "commandline")
		{
			ignoreLine(file, LengthofFile);
		}
		else if (Key == "numJoints")
		{
			file >> m_numJoints;
		}
		else if (Key == "numMeshes")
		{
			file >> m_numMeshes;
		}
		else if (Key == "joints")
		{
			Joint joint;
			file >> Skip;
			for (size_t i = 0; i < m_numJoints; i++)
			{
				file >> joint.m_Name;

				if (joint.m_Name[joint.m_Name.size() - 1] != '"')
				{
					char checkChar;
					bool jointNameFound = false;
					while (!jointNameFound)
					{
						checkChar = file.get();

						if (checkChar == '"')
							jointNameFound = true;

						joint.m_Name += checkChar;
					}
				}

				file>>joint.m_Parent >> Skip >>
					joint.m_Position.x >> joint.m_Position.y >> joint.m_Position.z >> Skip >>
					Skip >> joint.m_Orient.x >> joint.m_Orient.y >> joint.m_Orient.z;

				removeQuatos(joint.m_Name);
				joint.ComputeQuatW();

				m_Joints.push_back(joint);

				ignoreLine(file, LengthofFile);
			}
			file >> Skip;
		}
		else if (Key == "mesh")
		{
			file >> Skip;
			Mesh mesh;
			file >> Key;
			while (Key != "}")
			{
				if (Key == "shader")
				{
					file >> mesh.m_ShaderName;
					removeQuatos(mesh.m_ShaderName);
					mesh.m_ShaderName += ".tga";
				}
				else if (Key == "numverts")
				{
					file >> mesh.m_NumVertices;
					ignoreLine(file, LengthofFile);
					for (size_t i = 0; i < mesh.m_NumVertices; i++)
					{
						Vertex vertex;
						file >> Skip >> Skip >> Skip >>
							vertex.m_Tex0.x >> vertex.m_Tex0.y >> Skip >>
							vertex.m_StartWeight >> vertex.m_WeightCount;
						ignoreLine(file, LengthofFile);

						mesh.m_Vertices.push_back(vertex);
					}
				}
				else if (Key == "numtris")
				{
					file >> mesh.m_NumTris;
					ignoreLine(file, LengthofFile);

					for (size_t i = 0; i < mesh.m_NumTris; i++)
					{
						unsigned int vert1, vert2, vert3;
						file >> Skip >> Skip >>
							vert1 >> vert2 >> vert3;
						ignoreLine(file, LengthofFile);
						mesh.m_indicesTriangles.push_back(vert1);
						mesh.m_indicesTriangles.push_back(vert2);
						mesh.m_indicesTriangles.push_back(vert3);
					}
				}
				else if (Key == "numweights")
				{
					file >> mesh.m_NumWeights;
					ignoreLine(file, LengthofFile);

					for (size_t i = 0; i < mesh.m_NumWeights; i++)
					{
						Weight weight;
						file >> Skip >> Skip >>
							weight.m_JointID >> weight.m_Bias >> Skip >>
							weight.m_Pos.x >> weight.m_Pos.y >> weight.m_Pos.z;

						ignoreLine(file, LengthofFile);
						mesh.m_Weights.push_back(weight);
					}
					mesh.m_Weights.size();
				}
				else
				{
					ignoreLine(file, LengthofFile);
				}
				file >> Key;
			}
			ComputePositions(mesh);
			ComputeNormals(mesh);
			mesh.Setup();
			m_Meshs.push_back(mesh);
		}
		file >> Key;
	}
}

bool ModelMD5::loadModelAnim(const std::string& fileName)
{
	m_Anim.loadModelAnim(fileName);
	return true;
}


void ModelMD5::Draw(float DeltaTime)
{
	if (m_Anim.AnimActive)
	{
		Update(DeltaTime);
		for (size_t i = 0; i < m_numMeshes; i++)
		{
			m_Meshs.at(i).Setup();
			m_Meshs.at(i).Draw();
		}
	}
	else
	{
		for (size_t i = 0; i < m_numMeshes; i++)
		{
			m_Meshs.at(i).Draw();
		}
	}
}

void ModelMD5::ComputePositions(Mesh& mesh)
{
	mesh.m_Positions.clear();
	mesh.m_2DTextureCoord.clear();

	for (size_t i = 0; i < mesh.m_Vertices.size(); i++)
	{
		glm::vec3 finalVertex = { 0.0f, 0.0f, 0.0f };

		for (size_t j = 0; j < mesh.m_Vertices.at(i).m_WeightCount; j++)
		{
			Weight* weight = &mesh.m_Weights[mesh.m_Vertices.at(i).m_StartWeight + j];
			Joint* joint = &m_Joints.at(weight->m_JointID);

			glm::vec3 rotPos = joint->m_Orient * weight->m_Pos;

			finalVertex += (joint->m_Position + rotPos) * weight->m_Bias;
		}
		mesh.m_Vertices.at(i).Position = finalVertex;
		mesh.m_Positions.push_back(finalVertex);
		mesh.m_2DTextureCoord.push_back(mesh.m_Vertices.at(i).m_Tex0);
	}
}



void ModelMD5::ComputeNormals(Mesh& mesh)
{
	for (size_t i = 0; i < mesh.m_NumTris; i++)
	{
		glm::vec3 p0 = mesh.m_Vertices.at(mesh.m_indicesTriangles[i * 3]).Position;
		glm::vec3 p1 = mesh.m_Vertices.at(mesh.m_indicesTriangles[i * 3 + 1]).Position;
		glm::vec3 p2 = mesh.m_Vertices.at(mesh.m_indicesTriangles[i * 3 + 2]).Position;

		glm::vec3 Normal = glm::cross(p2 - p0, p1 - p0);

		mesh.m_Vertices[mesh.m_indicesTriangles[i * 3]].Normal += Normal;
		mesh.m_Vertices[mesh.m_indicesTriangles[i * 3 + 1]].Normal += Normal;
		mesh.m_Vertices[mesh.m_indicesTriangles[i * 3 + 2]].Normal += Normal;
	}

	for (size_t i = 0; i < mesh.m_Vertices.size(); i++)
	{
		glm::vec3 normal = glm::normalize(mesh.m_Vertices[i].Normal);
		mesh.m_Normals.push_back(normal);
	}
}

void ModelMD5::Update(float DeltaTime)
{
	m_Anim.Update(DeltaTime);
	const std::vector<Joint> skeleton = m_Anim.m_AnimSkelaton;
	if (skeleton.size()>=1)
	{
		for (unsigned int i = 0; i < m_Meshs.size(); ++i)
		{
			ComputePositions(m_Meshs[i], skeleton);
		}
	}
	
}
void ModelMD5::ClearModel()
{
	m_Joints.clear();
	m_Meshs.clear();
}
void ModelMD5::ComputePositions(Mesh& mesh, const std::vector<Joint>& skel)
{
	for (unsigned int i = 0; i < mesh.m_Vertices.size(); ++i)
	{
		const Vertex& vert = mesh.m_Vertices[i];
		glm::vec3& pos = mesh.m_Positions[i];
		glm::vec3& normal = mesh.m_Normals[i];

		pos = glm::vec3(0);
		normal = glm::vec3(0);

		for (int j = 0; j < vert.m_WeightCount; ++j)
		{
			const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
			const Joint& joint = skel[weight.m_JointID];

			glm::vec3 rotPos = joint.m_Orient * weight.m_Pos;
			pos += (joint.m_Position + rotPos) * weight.m_Bias;

			normal += (joint.m_Orient * vert.Normal) * weight.m_Bias;
		}
		
	}
}
glm::vec3 ModelMD5::getLookAt()
{
	glm::vec3 ptr(0.0);
	unsigned int vxCount=0;
	for (size_t i = 0; i < m_numMeshes; i++)
	{
		for (size_t y = 0; y < m_Meshs[i].m_Positions.size(); y++)
		{
			ptr += m_Meshs[i].m_Positions.at(y);
			vxCount +=1;
		}
	}
	ptr.x = ptr.x / vxCount;
	ptr.y = ptr.y / vxCount;
	ptr.z = ptr.z / vxCount;
	return ptr;
}
float ModelMD5::getMaxSize()
{
	float max=0;

	for (size_t i = 0; i < m_numMeshes; i++)
	{
		for (size_t y = 0; y < m_Meshs[i].m_Positions.size(); y++)
		{
			if (max < abs(m_Meshs[i].m_Positions.at(y).x))
			{
				max = abs(m_Meshs[i].m_Positions.at(y).x);
			}
			if (max < abs(m_Meshs[i].m_Positions.at(y).y))
			{
				max = abs(m_Meshs[i].m_Positions.at(y).y);
			}
			if (max < abs(m_Meshs[i].m_Positions.at(y).z))
			{
				max = abs(m_Meshs[i].m_Positions.at(y).z);
			}
		}
	}
	
	return max;
}

void ModelMD5::setAnimActive()
{
	m_Anim.AnimActive = true;
}

void ModelMD5::ClearAnim()
{
	m_Anim.ClearAll();
	m_Anim.AnimActive = false;
}
