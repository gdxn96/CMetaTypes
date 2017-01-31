// TemplateMetaprogramming.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <fstream>
#include "../../../MetaSystem/MetaSystem.h"

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
	ADD_RE_MEMBER(name);

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
	ADD_RE_MEMBER(george);
	ADD_RE_MEMBER(frankie);
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
	ADD_RE_MEMBER(x);
	ADD_RE_MEMBER(y);
	ADD_RE_MEMBER(shane);
}

class Object : public RuntimeEditable<Object>
{
public: 
	META_DATA(Object);
	Object() { RuntimeEditable::jsonInit(); };
	Object(int _x) : x(_x) { RuntimeEditable::jsonInit(); };
private:
	const int x = 3, y = 4, z = 5;
	std::string a = "aaa";
	Vec v;
	long j; 
	bool exampleBool = false;
};

DEFINE_META(Object)
{
	ADD_RE_MEMBER(z);
	ADD_RE_MEMBER(y);
	ADD_RE_MEMBER(x);
	ADD_RE_MEMBER(a);
	ADD_RE_MEMBER(v);
	ADD_RE_MEMBER(j);
	ADD_RE_MEMBER(exampleBool);
	LINK_TO_JSON(Object);
}



int main()
{
	Object* x = new Object();
	Object* y = new Object(42);
	
	std::string json = Variable(x).ToJson();
	std::cout << json << std::endl;

	json = Variable(y).ToJson();
	Variable(y).FromJson<Object>(json);
	std::cout << json << std::endl;


	std::cout << "Edit your json file" << std::endl;
	std::cin.get();

	FileMonitor::getInstance().MonitorFiles(6);

	auto& list = EntityList::get<Object>();
	list;
	
	json = Variable(x).ToJson();
	std::cout << json << std::endl;

	json = Variable(y).ToJson();
	std::cout << json << std::endl;

	system("PAUSE");
}

