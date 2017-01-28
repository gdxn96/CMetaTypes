// TemplateMetaprogramming.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "Meta.h"
#include <fstream>



class Student
{
public:
	META_DATA(Student);
private:
	const char* name = "student";
};

DEFINE_META(Student)
{
	ADD_MEMBER(name);
}

class Shane
{
public:
	META_DATA(Shane);
private:
	Student george, frankie;
};

DEFINE_META(Shane)
{
	ADD_MEMBER(george);
	ADD_MEMBER(frankie);
}

#define var(x) RefVariable(#x, &x)

class Vec
{
public:
	META_DATA(Vec);
private:
	float x = 10, y = 10;
	Shane shane;
};

DEFINE_META(Vec)
{
	ADD_MEMBER(x);
	ADD_MEMBER(y);
	ADD_MEMBER(shane);
}

class Object
{
public: 
	META_DATA(Object);
	const int x = 3, y = 4, z = 5;
	std::string a = "aaa";
	const Vec v;
	long j;
};

DEFINE_META(Object)
{
	ADD_MEMBER(z);
	ADD_MEMBER(y);
	ADD_MEMBER(x);
	ADD_MEMBER(a);
	ADD_MEMBER(v);
	ADD_MEMBER(j);
}

std::string ReadFile(std::string filePath)
{
	std::string contents;
	std::string line;
	std::ifstream myfile("example.json");
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
		std::cout << "Unable to open file" << std::endl;
	}

	return contents;
}



int main()
{
	Object x;
	
	std::string json = Variable(&x).ToJson();
	std::cout << json << std::endl;
	

	std::ofstream myfile;
	myfile.open("example.json");
	myfile << json;
	myfile.close();

	std::cin.get();

	json = ReadFile("example.json");

	Variable(&x).FromJson<Object>(json);
	json = Variable(&x).ToJson();
	std::cout << json << std::endl;
	
	int realValue = 42;

	int i = 12345;
	int j = 1234567890;
	const char* string = "2424";
	int vector = 0;
	float f = 2.5f;

	/*Variable a = var(f);
	Variable b = var(i);

	a = b;*/

	//std::cout << a << std::endl;

	/*std::vector<RefVariable> myVariables;
	
	myVariables.push_back(var(i));
	myVariables.push_back(var(realValue));
	myVariables.push_back(var(f));
	myVariables.push_back(var(string));*/

	/*for (RefVariable var : myVariables)
	{
		std::cout << var.type->Name() << " " <<  var.name << " = " << var.type->ToString(var.address) << std::endl;
	}*/
	//int& test = myVariables[0].get<int>();
	//test = 8;

	//string = meta::Cast<char*>(&i, meta::GetMetaType(i));

	//std::cout << meta::GetMetaType("kjdbvbi").Name() << std::endl;

	system("PAUSE");
}

