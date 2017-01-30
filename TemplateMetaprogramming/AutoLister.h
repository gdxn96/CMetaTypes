#pragma once
#include <vector>
#include <algorithm>

template<typename T>
class AutoLister
{
public:
	AutoLister() 
	{
		AddElement<T>(static_cast<const T *>(this));
	}

	template<typename T>
	static void RemoveElement(const T* element)
	{
		auto& instances = getInstances<T>();
		instances.erase(std::remove(instances.begin(), instances.end(), element), instances.end());
	}

	template<typename T>
	static void AddElement(const T* element)
	{
		auto& instances = getInstances<T>();
		instances.push_back(element);
	}

	virtual ~AutoLister() 
	{
		RemoveElement<T>(static_cast<const T *>(this));
	}

	template<typename T>
	static std::vector<const T*>& getInstances()
	{
		static std::vector<const T*> instances;
		return instances;
	}
};