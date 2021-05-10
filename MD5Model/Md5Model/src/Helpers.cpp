#include "Helpers.hpp"

int GetFileLength(std::istream& file)
{
    file.seekg(0, file.end);
    int length = file.tellg();
    file.seekg(0, file.beg);

    return length;
}
void ignoreLine(std::istream& file, int Length)
{
    file.ignore(Length, '\n');
}
void removeQuatos(std::string& Name)
{
    std::string newWord;
    for (char let : Name)
    {
        if (let != '\"')
        {
            newWord += let;
        }
    }
    Name = newWord;
}
