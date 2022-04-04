#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include "Table.h"

class CFileControl
{
public:
	CFileControl(std::istream& input, std::ostream& output);
	CTable ReadTable();
	void WriteTable(CTable::DeterministicAutomaton table, std::set<std::string> actions);
private:
	void PrintElement(const std::string& str);

	std::istream& m_input;
	std::ostream& m_output;
};