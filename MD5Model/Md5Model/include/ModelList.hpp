#pragma once
#include<vector>
#include<string>
class ModelList
{
public:
	std::vector<std::string> ModelPathList;
	std::vector<std::string> AnimPathList;

	void setAnimList(int index);
	void setModelList(std::string Path);
};