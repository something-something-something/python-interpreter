using namespace std;
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "python.tab.h"
#include <vector>
#include "ProgramTree.hpp"
#include "EvalMachine.hpp"
int yylex();
void lnumber();
vector<ProgramTree*> getStatmentList();
void yylex_destroy();
extern FILE* yyin;
void delstringsflex();
void delstringsbison();

void buildParseTree(vector<ProgramTree*> pList,int vectPos,ProgramTree* currentParent){
	if(currentParent->getType()==program){
		currentParent->addChild(pList[vectPos]);
	}
	else if(currentParent->getType()==ifStatments&&pList[vectPos]->getType()==elseStatments){
		if(currentParent->getIndent()>=pList[vectPos]->getIndent()){
			while(currentParent->getIndent()>=pList[vectPos]->getIndent() && currentParent->getType()!=program&&
			!(currentParent->getType()==ifElSt&&currentParent->getIndent()==pList[vectPos]->getIndent())){
				currentParent=currentParent->getParent();
			}
		}
		currentParent->addChild(pList[vectPos]);
	}
	
	else if(currentParent->getType()==functionStatments||currentParent->getType()==elseStatments||currentParent->getType()==ifStatments){
		if(currentParent->getIndent()>=pList[vectPos]->getIndent()){
			while(currentParent->getIndent()>=pList[vectPos]->getIndent() && currentParent->getType()!=program){
				currentParent=currentParent->getParent();
			}
		}
		currentParent->addChild(pList[vectPos]);
	}
	
	if(pList[vectPos]->getType()==ifElSt){
		ProgramTree* theIfStatments=new ProgramTree(pList[vectPos]->getIndent(),ifStatments);
		pList[vectPos]->addChild(theIfStatments);

		currentParent=theIfStatments;

	}
	if(pList[vectPos]->getType()==functionDecleration){
		ProgramTree* theFunctionStatments=new ProgramTree(pList[vectPos]->getIndent(),functionStatments);
		pList[vectPos]->addChild(theFunctionStatments);

		currentParent=theFunctionStatments;

	}
	if(pList[vectPos]->getType()==elseStatments){
		

		currentParent=pList[vectPos];

	}



	vectPos++;

	if(vectPos<pList.size()){
		buildParseTree(pList,vectPos,currentParent);
	}
}
int main(int argc, char *argv[]){
	//yyparse();
	//while(yylex());
	//cout<<"TEST"<<endl;
	//cout<<argv[1]<<endl;
	FILE* pythonFile= fopen(argv[1],"r");
	yyin=pythonFile;
	yyparse();
	//lnumber();
	vector<ProgramTree*> theList=getStatmentList();
	ProgramTree* theProgram=new ProgramTree(0,program);
	buildParseTree(theList,0,theProgram);
	//theProgram->print();
	vector<Variable> vars;
	vector<Function> funcs;
	EvalMachine theMachine=EvalMachine(vars,funcs);
	try{
		theMachine.evaluate(theProgram);
	}
	catch(string e){
		cout<<e<<endl;
	}
	yylex_destroy();
	delete theProgram;
	delstringsflex();
	delstringsbison();
	fclose(pythonFile);
	return 0;
}


