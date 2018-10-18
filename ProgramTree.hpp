#ifndef PROGTREE
#define PROGTREE

using namespace std;
#include <stdlib.h>
#include <string>
#include <vector>
//type of statment/part of statment
enum ParseTreeType{
	varAssignSt,/*variable assignment ex: x=3 contains a mathSt that is evaluated to get variable value. stringValue is varable name*/
	ifElSt,/*An if else statment. contains ifBool (boolean statment), ifStatments (statments to run if ifBool is true), and optionaly elseStatments (statments to run if ifBool is false)*/
	ifBool,/*boolean statment contains two children that are either intConst or variable. stringValue is the comparison operator*/
	variable,//used for ifBool. stringValue is variable name
	intConst,//used for ifBool. intValue is integer value
	mathSt,/*used for mathmatical statments. also function calls. stringValue is the mathmatical operation/function call. This is evaluated during evaluate()*/
	ifStatments,//statments to run if ifBool is true
	elseStatments,//statments to run if ifBool is true
	printSt,//print statment. Prints the value of it's children
	str,//a string. stringValue is the string
	program,//root of the tree. can have almost anything as a child
	functionDecleration,//function decleration. contains a functionStatments which holds the statments to evaluate when a function is called. stringValue is function name
	functionStatments,//statments to run when function is called. an have almost anything as a child
	returnStatment//for return statments in functions. contains a mathSt
	};
class ProgramTree{
	public:
		ProgramTree(int indent,ParseTreeType theType);
		//get indentation level
		int getIndent();
		ParseTreeType getType();
		ProgramTree* getParent();
		vector<ProgramTree*> getChildren();
		void setIntValue(int i);
		void setStringValue(string s);
		void setParent(ProgramTree* p);
		ProgramTree* getChild(int i);
		string getStringValue();
		int getIntValue();
		void print();
		void addChild(ProgramTree* p);
		~ProgramTree();
		ProgramTree* getScope();
	protected:
		int indent;
		ParseTreeType treeType;
		vector<ProgramTree*> children;
		ProgramTree* parent;
		//stores an int value (usage depends on type)
		int intValue;
		//stores an string value (usage depends on type)
		string stringValue;
};
#endif