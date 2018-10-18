using namespace std;
#include <stdlib.h>
#include <string>
#include <vector>
#include "ProgramTree.hpp"

//mathmatical statments get split into these
//can be a function variable int or operator
struct MathComponent{
	string name;
	int value;
	char symbol;
	bool isFunc;
};
//a variable
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
		//highest element in tree that can get the variables value
		ProgramTree* scope;
};
//a function
class Function{
	public:
		Function(string n, ProgramTree* loc,ProgramTree* s);
		void setLocation(ProgramTree* loc);
		ProgramTree* getLocation();
		string getName();
		ProgramTree* getScope();
	private:
		string name;
		//location of function in tree
		ProgramTree* location;
		ProgramTree* scope;
};
class EvalMachine{
	public:
		EvalMachine(vector<Variable> v,vector<Function> f);
		//asigns a variable a value
		void assignVariable(string name, int value,ProgramTree* scope);
		//stores the location of a function (used when evaluating a function definition)
		void assignFunction(string name, ProgramTree* loc,ProgramTree* scope);
		//runs the program
		int evaluate(ProgramTree* current);
		//int mathEval(string s);
		//gets value of variable given name and scope
		int getVariableValue(string name,ProgramTree *scope);
		//gets function location(used to call a function)
		ProgramTree* getFunctionLocation(string name,ProgramTree *scope);
		//converts a math component into an int also calls evaluate on a function
		int evaluateMathComponent(MathComponent m,ProgramTree* scope);
	private:
		vector<Variable> variables;
		vector<Function> functions;
		//ProgramTree* tree;

};
