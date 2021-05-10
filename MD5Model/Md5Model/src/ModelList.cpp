#include "ModelList.hpp"
#include <filesystem>
void ModelList::setModelList(std::string Path)
{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(Path))
    {
        if (entry.path().u8string().find(".md5mesh") != std::string::npos)
        {
            ModelPathList.push_back(entry.path().u8string());
        }
    }
    setAnimList(0);
}
void ModelList::setAnimList(int index)
{
    AnimPathList.clear();
    std::string Path = ModelPathList.at(index);
    int pathEnd = 0;
    std::string folderName = "";
    for (int i = Path.length() - 1; i > -1; i--)
    {
        if (Path.at(i) == '.')
        {
            i--;

            while (Path.at(i) != '\\')
            {
                folderName += Path.at(i);
                i--;
            }
            pathEnd = i;
            int n = folderName.length();

            for (int x = 0; x < n / 2; x++)
                std::swap(folderName[x], folderName[n - x - 1]);
            break;
        }
    }
    std::string path = Path.substr(0, pathEnd);
    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.path().u8string().find(".md5anim") != std::string::npos && entry.path().u8string().find(folderName) != std::string::npos)
        {
            AnimPathList.push_back(entry.path().u8string());
        }
    }
}
