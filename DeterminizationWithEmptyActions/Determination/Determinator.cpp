#include "Determinator.h"
#include <algorithm>
#include <iterator>
#include <set>
#include <iostream>

CDeterminator::CDeterminator(CTable& table)
	: m_table(table)
{
};

CTable::DeterministicAutomaton CDeterminator::Determinate()
{
	GetOutsForStartState();
	GetCompositeOuts();

	CTable::DeterministicAutomaton resultTable;
	for (auto& item : m_resultTable)
	{
		resultTable[{m_newStatesName[item.first.first], item.first.second}] = m_newStatesName[item.second];
	}
	return resultTable;
}

void CDeterminator::GetOutsForStartState()
{
	std::set<std::string> closure = m_table.GetClosureForFirstState();
	CTable::NondeterministicAutomaton nondeterministicTable = m_table.GetTable();
	m_newStatesName[m_table.GetStartState()] = "S0";
	for (const std::string& closureState : closure ) 
	{
		for (const std::string& action : m_table.GetActions())
		{
			for (CTable::NondeterministicAutomaton::iterator it = std::find_if(nondeterministicTable.begin(), nondeterministicTable.end(),
				[=](const auto& el) {return el.first.first == closureState && el.first.second == action && action != ""; });
				it != nondeterministicTable.end();
				it = std::find_if(++it, nondeterministicTable.end(), [=](const auto& el) {return el.first.first == closureState && el.first.second == action && action != ""; }))
			{
				m_resultTable[{m_table.GetStartState(), action}].append(it->second); 
				std::sort(m_resultTable[{m_table.GetStartState(), action}].begin(), m_resultTable[{m_table.GetStartState(), action}].end());
				auto res = std::unique(m_resultTable[{m_table.GetStartState(), action}].begin(), m_resultTable[{m_table.GetStartState(), action}].end());
				m_resultTable[{m_table.GetStartState(), action}] = std::string(m_resultTable[{m_table.GetStartState(), action}].begin(), res);
			}
		}
	}
	for (auto& item : m_resultTable)
	{
		m_foundStates.insert(item.second);

		if (m_newStatesName.find(item.second) == m_newStatesName.end() && item.second != "")
		{
			m_newStatesName[item.second] = "S" + std::to_string(m_newStatesName.size());
		}
	}
}

void CDeterminator::GetCompositeOuts()
{
	CTable::NondeterministicAutomaton nondeterministicTable = m_table.GetTable();
	std::set<std::string> actions = m_table.GetActions();
	bool isNewCompositeState = true;
	std::set<std::string> closure;

	while (isNewCompositeState)
	{
		isNewCompositeState = false;
		for (auto& item : m_resultTable)
		{
			CTable::DeterministicAutomaton::iterator it = std::find_if(m_resultTable.begin(), m_resultTable.end(),
				[item](const auto& el) {return el.first.first == item.second; });
			if (it == m_resultTable.end())
			{
				closure = m_table.GetClosureByState(item.second);
				for (const std::string& closureState : closure)
				{
					for (const std::string& action : actions)
					{
						for (CTable::NondeterministicAutomaton::iterator it = std::find_if(nondeterministicTable.begin(), nondeterministicTable.end(),
							[=](const auto& el) {return el.first.first == closureState && el.first.second == action && action != ""; });
							it != nondeterministicTable.end();
							it = std::find_if(++it, nondeterministicTable.end(), [=](const auto& el) {return el.first.first == closureState && el.first.second == action && action != ""; }))
						{ 
							m_resultTable[{item.second, action}].append(it->second);
							std::sort(m_resultTable[{m_table.GetStartState(), action}].begin(), m_resultTable[{m_table.GetStartState(), action}].end());
							auto res = std::unique(m_resultTable[{m_table.GetStartState(), action}].begin(), m_resultTable[{m_table.GetStartState(), action}].end());
							m_resultTable[{m_table.GetStartState(), action}] = std::string(m_resultTable[{m_table.GetStartState(), action}].begin(), res);
							if (m_foundStates.find(it->second) == m_foundStates.end())
							{
								isNewCompositeState = true;
							}
						}
					}
				}
				for (auto& item : m_resultTable)
				{
					m_foundStates.insert(item.second);
					if (m_newStatesName.find(item.second) == m_newStatesName.end() && item.second != "")
					{
						m_newStatesName[item.second] = "S" + std::to_string(m_newStatesName.size());
					}
				}
			}
		}
	}
}
