#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
class Joint
{
public:
	std::string m_Name;
	int m_Parent;
	glm::vec3 m_Position;
	glm::quat m_Orient;
	void ComputeQuatW();
};
class JointHierarchy
{
public:
	std::string		m_jointName;
	int				m_parent;
	int				flags;
	int				startIndex;
};