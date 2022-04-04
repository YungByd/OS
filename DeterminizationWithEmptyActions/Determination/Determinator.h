#pragma once
#include <string>
#include <map>
#include "Table.h"

class CDeterminator
{
public:
	CDeterminator(CTable& table);
	CTable::DeterministicAutomaton Determinate();

private:
	void GetOutsForStartState();
	void GetCompositeOuts();

	std::set<std::string> m_foundStates;
	std::map<std::string, std::string> m_newStatesName;
	CTable& m_table;
	CTable::DeterministicAutomaton m_resultTable;
};