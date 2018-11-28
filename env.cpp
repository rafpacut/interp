#include "env.hpp"
#include <algorithm>
#include <boost/any.hpp>

namespace ast
{
	Environment::Environment()
	{
		this->createScope();
	}

	int Environment::getValue(const std::string& name, optional<unsigned int> idx) const
	{
		auto res = std::find_if(scopes.crbegin(), scopes.crend(),
				[&name](Scope const& s){ return s.hasVariable(name);});

		if(res == scopes.crend())
			throw std::runtime_error("Using uninitialized variable "+name);

		return res->getValue(name, idx);
	}

	void Environment::createScope()
	{
		scopes.push_back(Scope()); 
	}

	void Environment::deleteScope()
	{
		if(scopes.size() == 1)
			throw std::runtime_error("Global scope cannot be deleted.");
		scopes.pop_back();
	}

	void Environment::copyValue(const std::string& fromName, const std::string& toName)
	{
		auto fromScopeIt = std::find_if(scopes.crbegin(), scopes.crend(),
				[&fromName](Scope const& s){ return s.hasVariable(fromName);});

		auto toScopeIt = std::find_if(scopes.rbegin(), scopes.rend(),
				[&toName](Scope const& s){ return s.hasVariable(toName);});

		if(fromScopeIt == scopes.rend())
			throw std::runtime_error("Cannot find variable with name "+fromName);
		if(toScopeIt == scopes.rend())
			throw std::runtime_error("Cannot find variable with name "+toName);

		try
		{
			toScopeIt->ints.at(toName) = fromScopeIt->ints.at(fromName);
		}
		catch(...)
		{
			try
			{
				toScopeIt->intVecs.at(toName) = fromScopeIt->intVecs.at(fromName);
			}
			catch(...)
			{
				throw std::runtime_error("Env::copyValue");
			}
		}
	}

	int Scope::getValue(const std::string& name, const optional<unsigned int> idx) const
	{
		//Thats not dry nor pretty.
		if(idx) //if looking for array. Incorrect: if passed index.
		{
			auto vecPtr = intVecs.find(name);
			//if we found a vec in this Scope. Redundant with Environment::getValue
			//but let's keep it here just in case.
			//if vector is initialized at all
			//if it has a value at idx.
			if(vecPtr != intVecs.end() && vecPtr->second && vecPtr->second->at(*idx))
				return vecPtr->second->at(*idx);
			else
				throw std::runtime_error("Using uninitialized variable: "+name);
		}
		else
		{
			return *(ints.at(name));
		}
	}

	void Scope::assignValue(const std::string name, const optional<std::vector<int>>& value)
	{
		auto vecIt = intVecs.find(name);
		if(vecIt == intVecs.end())
			throw std::runtime_error("Cannot copy an array into an int.");

		vecIt->second = value;	
	}

	void Scope::assignValue(const std::string name, const int& value, const optional<unsigned int> idx)
	{
		if(idx)
		{
			auto& vec = intVecs.at(name);
			
			if(!vec)//we called assign value to an uninitialized vector.
			{
				intVecs.at(name) = std::vector<int>();
			}

			if(*idx > vec->size()+1)
				throw std::runtime_error("Index much greater than vector size");
			if(*idx == vec->size())
				vec->push_back(value);
			else
				vec->at(*idx) = value;
		}
		else
			ints.at(name) = value;
	}

	void Scope::insertValue(const std::string& name, const optional<int> value)
	{
		ints.insert({name,value});
	}

	void Scope::insertValue(const std::string& name, const optional<std::vector<int> > value)
	{
		intVecs.insert({name, value});
	}

	bool Scope::hasVariable(const std::string& name) const
	{
		return declaredNames.find(name) != declaredNames.end();
	}

}

























