#pragma once
#include <glm/glm.hpp>

class Vertex
{
public:
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2   m_Tex0;
	int         m_StartWeight;
	int         m_WeightCount;
};