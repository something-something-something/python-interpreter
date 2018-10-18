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

//creates a single tree to evaluate from a vector of trees
void buildParseTree(vector<ProgramTree*> pList,int vectPos,ProgramTree* currentParent){
	//adds element of vector to apropriate part of tree
	//uses indent levels and type to tell if it should be added above the current 
	//statment that is excepting children
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
	//changes statment that future statments should be added to as children
	//example if just added a function definition future elements should be children of the function (changes statment excepting children)
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
	//read file
	FILE* pythonFile= fopen(argv[1],"r");
	yyin=pythonFile;
	//parse file
	yyparse();
	//lnumber();
	//vector of parsed lines of the program
	vector<ProgramTree*> theList=getStatmentList();
	//root of tree to be built
	ProgramTree* theProgram=new ProgramTree(0,program);
	//creates a tree using indentation of statments
	buildParseTree(theList,0,theProgram);
	//theProgram->print();
	//to keep track of functions and variables
	vector<Variable> vars;
	vector<Function> funcs;
	//create object for evaluating
	EvalMachine theMachine=EvalMachine(vars,funcs);
	try{
		theMachine.evaluate(theProgram);
	}
	catch(string e){
		//for undefined functions and variables
		cout<<e<<endl;
	}
	//clean up
	yylex_destroy();
	delete theProgram;
	//delete strings created during lexing
	delstringsflex();
	//delete strings made during parsing
	delstringsbison();
	fclose(pythonFile);
	return 0;
}


