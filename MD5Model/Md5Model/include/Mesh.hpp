#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "ShaderProgram.hpp"
#include "Vertex.hpp"
#include "Weight.hpp"
class Mesh
{
public:
	std::string m_ShaderName;

	std::vector<Vertex> m_Vertices;
	std::vector<Weight> m_Weights;

	unsigned int m_NumVertices;
	unsigned int m_NumTris;
	unsigned int m_NumWeights;
	
	GLuint m_TexID;

	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_2DTextureCoord;
	std::vector<unsigned int> m_indicesTriangles;

	Mesh();
	void Draw();
	void Setup();
private:
	GLuint VBO, VEO, VAO;
	
};
#endif 