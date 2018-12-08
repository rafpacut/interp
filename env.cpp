#include "env.hpp"
#include <algorithm>
#include <iterator>

namespace ast
{
	Environment::Environment()
	{
		this->createScope();
	}

	Environment::Environment(const Environment& e) : functions(e.functions)
	{
		this->scopes.push_back(e.scopes.front());
	}

	int Environment::getValue(const std::string& name, optional<unsigned int> idx) const
	{
		auto res = std::find_if(scopes.crbegin(), scopes.crend(),
				[&name](Scope const& s){ return s.hasVariable(name);});

		if(res == scopes.crend())
			throw std::runtime_error("Cannot find variable with name "+name);

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

	      auto& toInts = toScopeIt->ints;
	      auto& fromInts = fromScopeIt->ints;
	
	      auto& fromIntVecs = fromScopeIt->intVecs;
	      auto& toIntVecs = toScopeIt->intVecs;
	
	      //If are both ints
	      if(toInts.find(toName) != toInts.end() && fromInts.find(fromName) != fromInts.end())
	      {
	      	toScopeIt->ints.at(toName) = fromScopeIt->ints.at(fromName);
	      }
	      //if are both vectors
	      else if(toIntVecs.find(toName) != toIntVecs.end() && fromIntVecs.find(fromName) != fromIntVecs.end())
	      {
	      	toScopeIt->intVecs.at(toName) = fromScopeIt->intVecs.at(fromName);
	      }
	      else
	      	throw std::runtime_error("Cross-type assignment");
	}

	void Environment::declare(const Function& fun)
	{
		functions.push_back(fun);
	}


	int Scope::getValue(const std::string& name, const optional<unsigned int> idx) const
	{
		if(idx) 
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

	void Scope::assignValue(const std::string name, const int value, const optional<unsigned int> idx)
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

























