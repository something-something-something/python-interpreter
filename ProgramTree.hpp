#ifndef PROGTREE
#define PROGTREE

using namespace std;
#include <stdlib.h>
#include <string>
#include <vector>
enum ParseTreeType{
	varAssignSt,
	ifElSt,
	ifBool,
	variable,
	intConst,
	mathSt,
	ifStatments,
	elseStatments,//not supported currently
	printSt,
	str,
	program
	};
class ProgramTree{
	public:
		ProgramTree(int indent,ParseTreeType theType);
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
		int intValue;
		string stringValue;
};
#endif