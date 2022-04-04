#include "FileControl.h"
#include <exception>
#include <sstream>
#include <algorithm>
#include <set>
#include <iomanip>

CFileControl::CFileControl(std::istream& input, std::ostream& output)
	: m_input(input)
	, m_output(output)
{
}

CTable CFileControl::ReadTable()
{
	CTable::NondeterministicAutomaton table;
	std::set<std::string> actions;
	std::string str, state, action, out, firstState;
	bool isFirstState = false;

	while (getline(m_input, str))
	{
		std::istringstream iss(str);
		getline(iss, state, ' ');
		getline(iss, action, ' ');
		getline(iss, out, ' ');
		if (!isFirstState)
		{
			firstState = state;
			isFirstState = true;
		}
		table.emplace(std::make_pair(state, action), out);
		actions.insert(action);
	}

	return CTable(table, firstState, actions);
}

void CFileControl::WriteTable(CTable::DeterministicAutomaton table, std::set<std::string> actions)
{
	PrintElement("");
	m_output << std::left;
	for (auto& item : actions)
	{
		PrintElement(item);
	}
	m_output << std::endl;

	std::set<std::string> writtenStates;
	for (auto& item : table)
	{
		if (!writtenStates.count(item.first.first))
		{
			writtenStates.insert(item.first.first);
			PrintElement(item.first.first);
			m_output << std::left;
			for (auto& action : actions)
			{
				if (table.find({ item.first.first, action }) != table.end())
				{
					PrintElement(table[{ item.first.first, action }]);
				}
				else
				{
					PrintElement("");
				}
			}
			m_output << std::endl;
		}
	}

}

void CFileControl::PrintElement(const std::string& str)
{
	const char separator = ' ';
	const int width = 6;

	m_output << std::setw(width) << std::setfill(separator) << str;
}
