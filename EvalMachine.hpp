using namespace std;
#include <stdlib.h>
#include <string>
#include <vector>
#include "ProgramTree.hpp"
struct MathComponent{
	string name;
	int value;
	char symbol;
	bool isFunc;
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
class Function{
	public:
		Function(string n, ProgramTree* loc,ProgramTree* s);
		void setLocation(ProgramTree* loc);
		ProgramTree* getLocation();
		string getName();
		ProgramTree* getScope();
	private:
		string name;
		ProgramTree* location;
		ProgramTree* scope;
};
class EvalMachine{
	public:
		EvalMachine(vector<Variable> v,vector<Function> f);
		void assignVariable(string name, int value,ProgramTree* scope);
		void assignFunction(string name, ProgramTree* loc,ProgramTree* scope);
		int evaluate(ProgramTree* current);
		//int mathEval(string s);
		int getVariableValue(string name,ProgramTree *scope);
		ProgramTree* getFunctionLocation(string name,ProgramTree *scope);
		int evaluateMathComponent(MathComponent m,ProgramTree* scope);
	private:
		vector<Variable> variables;
		vector<Function> functions;
		//ProgramTree* tree;

};
