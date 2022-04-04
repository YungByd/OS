#pragma once
#include <string>
#include <map>
#include <set>
#include <vector>

class CTable
{
public:

	typedef std::multimap<std::pair<std::string, std::string>, std::string> NondeterministicAutomaton;
	typedef std::map<std::pair<std::string, std::string>, std::string> DeterministicAutomaton;

	CTable(NondeterministicAutomaton grammar, const std::string& startState, std::set<std::string> actions);
	NondeterministicAutomaton GetTable() const;
	std::set<std::string> GetActions() const;
	std::set<std::string> GetClosureByState(const std::string& state) const;
	std::set<std::string> GetClosureForFirstState() const;
	std::string GetStartState() const;

private:
	void FindRecursiveClosure(const std::string& mainState, const std::string& state);
	NondeterministicAutomaton m_table;
	std::set<std::string> m_actions;
	std::map<std::string, std::set<std::string>> m_closures;
	std::string m_startState;
};