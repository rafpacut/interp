#include "env.hpp"
#include <algorithm>
#include <iterator>

namespace ast
{
	Environment::Environment()
	{
		this->createScope();
	}

	Environment::Environment(const Environment& e) : scopes(e.scopes), functions(e.functions)
	{
	}

	Environment& Environment::operator=(Environment&& old)
	{
		this->scopes.push_back(old.scopes.front());
		this->functions = old.functions;

		return *this;
	}

	basicType Environment::getValue(const std::string& name, optional<size_t> idx) const
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
		//shouldn't ever throw, but...
		if(scopes.size() == 0)
			throw std::runtime_error("Deleting last scope");
		scopes.pop_back();
	}

	void Environment::declare(const Function& fun)
	{
		auto [it, succesfullyInserted] = functions.insert(fun);
		if(!succesfullyInserted)
			throw std::runtime_error("Redefinition of function "+fun.name);
	}

	basicType Scope::getValue(const std::string& name, const optional<size_t> idx) const
	{
		if(idx) 
		{
			auto vecPtr = intVecs.find(name);
			if(!vecPtr->second)
				throw std::runtime_error("Using uninitialized variable: "+name);
			auto vec = *(vecPtr->second);

			return vecPtr->second->at(*idx);
		}
		else
		{
			if(intVecs.find(name) != intVecs.end())
			{
				if(!intVecs.at(name)) throw std::runtime_error("Returning uninitialized variable "+name);
				return *(intVecs.at(name));
			} else if(ints.find(name) != ints.end())
			{
				if(!ints.at(name)) throw std::runtime_error("Returning uninitialized variable "+name);	
				return *(ints.at(name));
			}
		}
		throw std::runtime_error("Scope::getValue error. Dunno what it might mean");
	}

	void Scope::assignValue(const std::string name, const int value)
	{
		ints.at(name) = value;
	}

	void Scope::assignValue(const std::string name, const std::vector<int> value)
	{
		intVecs.at(name) = value;
	}

	void Scope::assignValue(const std::string name, const int value, const size_t idx)
	{
		auto& optionalVec = intVecs.at(name);
		if(!optionalVec)
			optionalVec = std::vector<int>();
		auto& vec = *optionalVec;
		if(idx > vec.size() + 1)
			throw std::runtime_error("Array "+name+" has no value at "+std::to_string(idx)+" (size:"+std::to_string(vec.size())+")");
		if(idx == vec.size())
			vec.push_back(value);
		else
			vec.at(idx) = value;
	}

	void Scope::assignValue(const std::string, const std::vector<int>, const size_t)
	{
		throw std::runtime_error("Trying to assign an array to array cell. Not yet implemented.");
	}

	void Scope::insertValue(const std::string& name, const optional<int> value)
	{
		ints.insert({name,value});
	}

	void Scope::insertValue(const std::string& name, const optional<std::vector<int> > value)
	{
		intVecs.insert({name, value});
	}

	void Scope::arrayExtend(std::string const& name, int const value)
	{
		auto& vec = intVecs.at(name);
		if(!vec) vec = std::vector<int>();

		(*vec).push_back(value);
	}

	bool Scope::hasVariable(const std::string& name) const
	{
		return declaredNames.find(name) != declaredNames.end();
	}
}

























