#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP
#include <string>
#include <map>
class TextureManager
{
public:
    static TextureManager* getInstance();
    unsigned int loadTexture(std::string fileName);
    void activateTexture(unsigned int level, std::string fileName);
    void unLoadTexture(std::string fileName);
    void unLoadAllTexture();
private:
    TextureManager();
    static TextureManager* m_Instance;
    std::map <std::string, unsigned int>  m_TextureMap;
};


#endif