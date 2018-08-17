#ifndef ENV_H 
#define ENV_H

#include <boost/optional.hpp>
#include <list>
#include <unordered_map>
#include <unordered_set>


struct Scope
{
	using boost::optional;

	template<typename T>
	void declare(const& std::string, const optional<T>);

	int getValue(const& std::string, const optional<int>) const;

	void assignValue(std::string, const int&, const optional<int>);

	void insertValue(const& std::string, const optional<int>);
	void insertValue(const& std::string, const optional<vector<int>>);

	bool hasVariable(const& std::string) const;


	std::unordered_map<std::string, optional<int>> ints;
	std::unordered_map<std::string, optional< vector<int> > intVecs;

	std::unordered_set<std::string> declaredNames;
};



struct Environment
{

	using boost::optional;

	template<typename T>
	void declare(const& std::string, const optional<T>);
	int getValue(const& std::string, optional<int> idx = boost::none) const;

	template<typename T>
	void assignValue(const& std::string, const T& value, optional<int> id = boost::none);

	void createScope();
	void deleteScope();


	std::list<Scope> scopes;
};

#endif
