#include "Mesh.hpp"
#include <glad/glad.h> 
#include<GLFW/glfw3.h>
#include "TextureManager.hpp"
#include<iostream>
Mesh::Mesh()
{

}

void Mesh::Draw()
{
	
	TextureManager::getInstance()->activateTexture(GL_TEXTURE0, m_ShaderName);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indicesTriangles.size(), GL_UNSIGNED_INT, &(m_indicesTriangles[0]));
	glBindVertexArray(0);
}

void Mesh::Setup()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VEO);

	size_t pSize = m_Positions.size() * sizeof(glm::vec3);
	size_t tSize = m_2DTextureCoord.size() * sizeof(glm::vec2);
	size_t nSize = m_Normals.size() * sizeof(glm::vec2);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,  pSize + tSize + nSize, 0 , GL_STATIC_DRAW);


	glBufferSubData(GL_ARRAY_BUFFER, 0, pSize, &m_Positions[0]);

	glBufferSubData(GL_ARRAY_BUFFER, pSize, tSize, &m_2DTextureCoord[0]);

	glBufferSubData(GL_ARRAY_BUFFER, pSize + tSize, nSize, &m_Normals[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(pSize));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(pSize + tSize));

	GLint size = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	if (pSize+tSize+nSize != size)
	{
		std::cout << "VBO hatali!";
		glDeleteBuffers(1, &VBO);

		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VEO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_indicesTriangles.size(), &m_indicesTriangles[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	int tex = TextureManager::getInstance()->loadTexture(m_ShaderName);

}
