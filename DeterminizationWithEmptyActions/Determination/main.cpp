#include <iostream>
#include <fstream>
#include "FileControl.h"
#include "Table.h"
#include "Determinator.h"

int main()
{
	std::ifstream input("input.txt");
	std::ofstream output("output.txt");

	CFileControl fileControl(input, output);

	CTable table = fileControl.ReadTable();

	CDeterminator determinator(table);
	fileControl.WriteTable(determinator.Determinate(), table.GetActions());
}
