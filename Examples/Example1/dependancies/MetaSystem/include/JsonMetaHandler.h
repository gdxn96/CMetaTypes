#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Meta.h"

namespace JsonHandler
{
	template<typename T>
	std::string ReadFile(std::string filePath)
	{
		std::string contents;
		std::string line;
		std::ifstream myfile(filePath);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				contents += line + "\n";
			}
			myfile.close();
		}
		else
		{
			contents = Variable(&T()).ToJson();

			std::ofstream f;
			f.open(filePath);
			f << contents;
			f.close();
		}

		return contents;
	}

	template<typename T>
	std::string& get(std::string filepath = "", bool forceReload=false)
	{
		static std::string json = "";
		static std::string mFilePath = "";

		if (forceReload)
		{
			if (filepath != "")
			{
				mFilePath = filepath;
			}
			json = ReadFile<T>(mFilePath);
		}
		return json;
	}
}

