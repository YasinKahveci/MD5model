#pragma once
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
int GetFileLength(std::istream& file);

void ignoreLine(std::istream& file, int Length);

void removeQuatos(std::string& Name);