#include "env.hpp"
#include <algorithm>

template<typename T>
void Environment::declare(const& std::string name, const optional<T> value)
{
	scopes.back()->declare(name, value);
}

int Environment::getValue(const& std::string name, optional<int> idx = boost::none) const
{
	auto res = std::find_if(scopes.crbegin(), scopes.crend(),
			[](Scope const& s){ return s.hasVariable(name);});

	if(res == scopes.crend())
		throw std::runtime_error("Using uninitialized variable "+name);

	return res->getValue(name, idx);
}

template<typename T>
void assignValue(const& std::string name, const T& value, const optional<int> idx = boost::none)
{
	auto res = std::find_if(scopes.crbegin(), scopes.crend(),
			[](const& Scope s){ return s.hasVariable(name);}
			);

	res->assignValue(name, value, idx);
}

void Scope::assignValue(std::string name, const int& value, const optional<int> idx)
{
	if(idx)
	{
		auto& vec = intVecs.at(name);
		if(idx > vec.size())
				vec.push_back(value);
			else
				vec.at(idx) = value;
	}
	else
		ints.at(name) = value;

}

int Scope::getValue(const& std::string name, const optional<int> idx) const
{
	if(idx)
		return *intVecs.find(name);
	else
		return *ints.find(name);
}

template<typename T>
void Scope::declare(const& std::string name, const optional<T> value)
{
	if(!hasVariable(name))
	{
		declaredNames.insert(name);
		insertValue(name, value);
	}
	else
		throw(std::runtime_error("Name "+name+" is already in use.");
}

void Scope::insertValue(const& std::string name, const optional<int> value)
{
	ints.insert(name,value);
}

void Scope::insertValue(const& std::string name, const optional<vector<int>> value)
{
	intVecs.insert(name, value);
}

bool Scope::hasVariable(const& std::string name) const
{
	return declaredNames.find(name) != declaredNames.end();
}



























