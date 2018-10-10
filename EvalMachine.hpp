using namespace std;
#include <stdlib.h>
#include <string>
#include <vector>
#include "ProgramTree.hpp"
struct MathComponent{
	string name;
	int value;
	char symbol;
};

class Variable{
	public:
		Variable(string n, int v,ProgramTree* s);
		void setValue(int v);
		int getValue();
		string getName();
		ProgramTree* getScope();
	private:
		string name;
		int value;
		ProgramTree* scope;
};
class EvalMachine{
	public:
		EvalMachine(vector<Variable> v);
		void assignVariable(string name, int value,ProgramTree* scope);
		int evaluate(ProgramTree* current);
		//int mathEval(string s);
		int getVariableValue(string name,ProgramTree *scope);
		int evaluateMathComponent(MathComponent m,ProgramTree* scope);
	private:
		vector<Variable> variables;
		//ProgramTree* tree;

};
