#include "Table.h"
#include <iostream>
#include <algorithm>

CTable::CTable(NondeterministicAutomaton table, const std::string& startState, std::set<std::string> actions)
	: m_table(table)
	, m_startState(startState)
	, m_actions(actions)
{
	for (const auto& el : table)
	{
		FindRecursiveClosure(el.first.first, el.first.first);
	}
};

CTable::NondeterministicAutomaton CTable::GetTable() const
{
	return m_table;
}

std::set<std::string> CTable::GetActions() const
{
	return m_actions;
}

std::set<std::string> CTable::GetClosureByState(const std::string& state) const
{
	std::set<std::string> res;
	for (const auto& simpleState : state)
	{
		if (m_closures.find({ simpleState }) != m_closures.end())
		{
			res.insert(m_closures.at({ simpleState }).begin(), m_closures.at({ simpleState }).end());
		}
	}
	return res;
}

std::set<std::string> CTable::GetClosureForFirstState() const
{
	return GetClosureByState(m_startState);
}

std::string CTable::GetStartState() const
{
	return m_startState;
}

void CTable::FindRecursiveClosure(const std::string& mainState, const std::string& state)
{
	m_closures[mainState].insert(state);
	for (NondeterministicAutomaton::iterator it = std::find_if(m_table.begin(), m_table.end(),
		[state](const auto& el) {return el.first.first == state && el.first.second == ""; });
		it != m_table.end();
		it = std::find_if(++it, m_table.end(), [state](const auto& el) {return el.first.first == state && el.first.second == ""; }))
	{
		m_closures[mainState].insert(it->second);
		FindRecursiveClosure(mainState, it->second);
	}
}
