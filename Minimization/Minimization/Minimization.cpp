#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct Element
{
    int multiplicity;
    int to;
    int out;
};

using Automaton = vector<vector<Element>>;
using EMatrix = map<int, vector<int>>;

EMatrix matrix;
Automaton ResultA;
vector<int> ResultI;
ifstream input;
string type;
int x, y, r;

void GetEquivalenceMatrix(Automaton& automaton, vector<int>& indexes, EMatrix& equivalenceMatrix)
{
    int k = 0;  
    equivalenceMatrix[k].push_back(indexes[0]);

    for (int i = 1; i < indexes.size(); i++)
    {
        bool isFound = false;
        EMatrix::iterator it = equivalenceMatrix.begin();

        while (it != equivalenceMatrix.end() && !isFound)
        {
            isFound = true;
            for (int j = 0; j < automaton.size(); j++)
            {
                if (automaton[j][it->second[0]].multiplicity != automaton[j][indexes[i]].multiplicity)
                { 
                    isFound = false;
                    it++;
                    break;
                }
            }
        }

        if (isFound)
        {
            equivalenceMatrix[it->first].push_back(indexes[i]);
        }
        else
        {
            k++;
            equivalenceMatrix[k].push_back(indexes[i]);
        }
    }
}

void OverrideAutomaton(Automaton& automaton, EMatrix& equivalenceMatrix)
{
    for (int i = 0; i < automaton.size(); i++)
    {
        for (int j = 0; j < automaton[0].size(); j++)
        {
            EMatrix::iterator it = equivalenceMatrix.begin();

            while (find((it->second).begin(), (it->second).end(), automaton[i][j].to) == (it->second).end())
            {
                it++;
            }
            automaton[i][j].multiplicity = it->first;
        }
    }
}

void GetResult(Automaton& automaton, EMatrix& ResultM)
{
    Automaton result(automaton.size(), vector<Element>(ResultM.size()));
    EMatrix::iterator it = ResultM.begin();
    
    for (int i = 0; i < result[0].size(); i++)
    {        
        for (int j = 0; j < result.size(); j++)
        {
            result[j][i] = automaton[j][it->second[0]];  

            EMatrix::iterator n = ResultM.begin();
            while (find((n->second).begin(), (n->second).end(), automaton[j][it->second[0]].to) == (n->second).end())
            {
                n++;
            }
            result[j][i].to = n->second[0];
        }
        ResultI.push_back(it->second[0]);
        it++;        
    }

    ResultA = result;
}

void Minimization(Automaton& automaton, bool& isMinimized, int& N)
{
    EMatrix equivalenceMatrix; 
    EMatrix::iterator it = matrix.begin();
    for (; it != matrix.end(); it++)
    {
        EMatrix matrixE;
        GetEquivalenceMatrix(automaton, it->second, matrixE);

        int k = equivalenceMatrix.size();
        for (auto arr : matrixE)
        {
            equivalenceMatrix[k] = arr.second;
            k++;
        }
    }
    matrix = equivalenceMatrix;

    OverrideAutomaton(automaton, equivalenceMatrix);
    if (equivalenceMatrix.size() == N)
    {
        isMinimized = true;
        GetResult(automaton, equivalenceMatrix);
    }
        
    N = equivalenceMatrix.size();
}

void ReadMilli(Automaton& a, ifstream& input)
{
    string lineS;
    string lineY;
    getline(input, lineS);

    for (int i = 0; i < a.size(); i++)
    {
        getline(input, lineS);
        for (int k = 0; k < lineS.length(); ++k)
        {
            if (isalpha(lineS[k]))
            {
                lineS[k] = ' ';
            }
        }
        getline(input, lineY);
        for (int k = 0; k < lineY.length(); ++k)
        {
            if (isalpha(lineY[k]))
            {
                lineY[k] = ' ';
            }
        }

        stringstream strS(lineS);
        stringstream strY(lineY);

        int ar = a[0].size();
        for (int j = 0; j < a[0].size(); j++)
        {
            int numS;
            strS >> numS;
            a[i][j].to = numS - 1;

            int numY;
            strY >> numY;
            a[i][j].multiplicity = numY - 1;
            a[i][j].out = numY - 1;
        }
    }
}

void ReadMurra(Automaton& a, ifstream& input)
{
    string lineS;
    string lineY;
    getline(input, lineS);

    getline(input, lineY);
    for (int k = 0; k < lineY.length(); ++k)
    {
        if (isalpha(lineY[k]))
        {
            lineY[k] = ' ';
        }
    }
    stringstream strY(lineY);

    for (int i = 0; i < a[0].size(); i++)
    {
        int numY;
        strY >> numY;

        for (int j = 0; j < a.size(); j++)
        {
            a[j][i].multiplicity = numY - 1;
            a[j][i].out = numY - 1;
        }
    }

    for (int i = 0; i < a.size(); i++)
    {
        getline(input, lineS);
        for (int k = 0; k < lineS.length(); ++k)
        {
            if (isalpha(lineS[k]))
            {
                lineS[k] = ' ';
            }
        }
        stringstream strS(lineS);

        for (int j = 0; j < a[0].size(); j++)
        {
            int numS;
            strS >> numS;
            a[i][j].to = numS - 1;
        }
    }
}

void ReadAutomaton(Automaton& a, ifstream& input, string& type)
{   
    if (type == "Ml")
    {
        ReadMilli(a, input);
    }
    else
    {
        ReadMurra(a, input);
    } 
}

void WriteMilli()
{
    ofstream output;
    output.open("output.txt");

    for (int i = 0; i < ResultI.size(); i++)
    {
        string out = "S" + to_string(ResultI[i] + 1);
        output << setw(20) << out;
    }
    output << endl;

    for (int i = 0; i < ResultA.size(); i++)
    {
        output << "X" << i + 1;
        for (int j = 0; j < ResultA[0].size(); j++)
        {
            string out = "[S" + to_string(ResultA[i][j].to + 1) + ", Y" + to_string(ResultA[i][j].out + 1) + "]";
            output << setw(20) << right << out;
        }
        output << endl;
    }
}

void WriteMurra()
{
    ofstream output;
    output.open("output.txt");

    for (int i = 0; i < ResultI.size(); i++)
    {
        string out = "Y" + to_string(ResultA[0][i].out + 1);
        output << setw(20) << out;
    }
    output << endl;

    for (int i = 0; i < ResultI.size(); i++)
    {
        string out = "S" + to_string(ResultI[i] + 1);
        output << setw(20) << out;
    }
    output << endl;

    for (int i = 0; i < ResultA.size(); i++)
    {
        output << "X" << i + 1;
        for (int j = 0; j < ResultA[0].size(); j++)
        {
            string out = "[S" + to_string(ResultA[i][j].to + 1) + "]";
            output << setw(20) << right << out;
        }
        output << endl;
    }
}

void WriteAutomaton(string& type)
{
    
    if (type == "Ml")
    {                
        WriteMilli();
    }
    else
    {
        WriteMurra();
    }
}

void ReadInput()
{
    input.open("input.txt");
    getline(input, type);
    input >> y >> x >> r;
}

int main()
{    
    ReadInput();
    
    Automaton automaton(x, vector<Element>(y));
    ReadAutomaton(automaton, input, type);

    bool isMinimized = false;
    int N = automaton[0].size();
    for (int i = 0; i < N; i++)
    {
        matrix[0].push_back(i);
    }

    while (!isMinimized)
    {
        Minimization(automaton, isMinimized, N);
    }

    WriteAutomaton(type);
}
