// TemplateMetaprogramming.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "Meta.h"
#include <fstream>
#include "AutoLister.h"

#define META_DEBUGGING true

class Student
{
public:
	META_DATA(Student);
private:
	std::string name = "student";
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

class Object : public AutoLister<Object>
{
public: 
	META_DATA(Object);

private:
	const int x = 3, y = 4, z = 5;
	std::string a = "aaa";
	Vec v;
	long j;
	bool exampleBool = false;
};

DEFINE_META(Object)
{
	ADD_MEMBER(z);
	ADD_MEMBER(y);
	ADD_MEMBER(x);
	ADD_MEMBER(a);
	ADD_MEMBER(v);
	ADD_MEMBER(j);
	ADD_MEMBER(exampleBool);
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
	int y = 7;
	
	std::string json = Variable(&x).ToJson();
	std::cout << json << std::endl;
	
	auto list = EntityList::get<Object>();

	list;

	std::ofstream myfile;
	myfile.open("example.json");
	myfile << json;
	myfile.close();

	std::cin.get();

	json = ReadFile("example.json");

	Variable(&x).FromJson<Object>(json);
	json = Variable(&x).ToJson();
	std::cout << json << std::endl;

	system("PAUSE");
}

