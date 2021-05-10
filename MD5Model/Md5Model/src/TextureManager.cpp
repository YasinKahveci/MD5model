#include "TextureManager.hpp"
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#include "KHR/stb_image.h"
#include <iostream>
TextureManager* TextureManager::m_Instance = 0;
TextureManager::TextureManager()
{
    //stbi_set_flip_vertically_on_load(true);
}
TextureManager* TextureManager::getInstance()
{
    if (m_Instance)
    {
        return m_Instance;
    }

    m_Instance = new TextureManager();

    return m_Instance;
}

unsigned int TextureManager::loadTexture(std::string fileName)
{

    if (m_TextureMap[fileName] != 0)
    {
        return m_TextureMap[fileName];
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, depth;

    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &depth, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "texture loaded: " + fileName << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture: " + fileName  << std::endl;
    }
    stbi_image_free(data);

    m_TextureMap[fileName] = texture;

    return texture;
}

void TextureManager::activateTexture(unsigned int level, std::string fileName)
{
    glActiveTexture(level);
    glBindTexture(GL_TEXTURE_2D, m_TextureMap[fileName]);
}

void TextureManager::unLoadTexture(std::string fileName)
{
    glDeleteTextures(1, &m_TextureMap[fileName]);
    m_TextureMap.erase(fileName);
}

void TextureManager::unLoadAllTexture()
{
    for (std::map<std::string, unsigned int>::iterator it = m_TextureMap.begin(); it != m_TextureMap.end(); ++it)
    {
        glDeleteTextures(1, &it->second);
    }
    m_TextureMap.clear();
}