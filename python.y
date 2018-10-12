
%{
using namespace std;
#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "ProgramTree.hpp"
extern int yylex();
extern int yyparse();
void yyerror(const char* s);
int linenumber=1;
vector<ProgramTree*> statmentList;
int currentIndent=0;


%}

%union{
	char* s;
	int intv;
	char charv;
	struct BoolStruct{
		string* firstVarname;
		int firstInt;
		string* compare;
		string* secondVarname;
		int secondInt;

};
	BoolStruct b;
	
}
//integer constant 3,4 etc
%token <intv> INT
//+-*/ (probbaly should be split into seprate tokens)
%token <s> MATHSYMBOL
%token <s> STRING
//identifier (variable or function)
%token <s> IDENT
//if identifier
%token <s> IFID
%token <s> FUNCDEF
%token <s> RETURNS
%token <s> BOOLSYMBOL
%token <s> ENDL
%token <intv> INDENT
%token <intv> PRINT
%type <s> math
%type <b> boolexpr
%%

lines:
	lend  lines | line 

lend:
	line ENDL {currentIndent=0;linenumber++;} | ENDL {currentIndent=0;linenumber++;}
line:
	linestatment  |
	INDENT {currentIndent=$1;} linestatment | 
	 |
	INDENT {currentIndent=$1;} 


linestatment:
	mathstatment |
	printstatment |
	varassign  |
	 ifstatment
	 | functiondef |
	 returnSt
	;


functiondef:
	FUNCDEF IDENT '(' ')' ':'{
		ProgramTree* func=new ProgramTree(currentIndent,functionDecleration);
		func->setStringValue($2);
		statmentList.push_back(func);
	}

//will be more complicted
mathstatment:
	math {
		//ProgramTree* mathy=new ProgramTree(currentIndent,mathSt);
		//mathy->setStringValue($1);
		//statmentList.push_back(mathy);
		}
math:
	INT  {string i=to_string($1);
		$$=strdup(i.c_str());
		}
	| 
	IDENT {$$=$1;}

	|
	IDENT '(' ')' {
		string funcall=$1;
		funcall=funcall+"()";
		$$=strdup(funcall.c_str());
	}

	|INT MATHSYMBOL math {
		string mathSt=to_string($1);
		mathSt+=$2;
		mathSt+=$3;
		$$=strdup(mathSt.c_str());}
	|
	IDENT MATHSYMBOL math {
		string mathSt=$1;
		mathSt+=$2;
		mathSt+=$3;
		$$=strdup(mathSt.c_str());}
	|
	IDENT '(' ')' MATHSYMBOL math {
		string funcall=$1;
		funcall=funcall+"()";
		string mathSt=strdup(funcall.c_str());
		mathSt+=$4;
		mathSt+=$5;
		$$=strdup(mathSt.c_str());}

printstatment:
	PRINT STRING ')'{
		ProgramTree* print=new ProgramTree(currentIndent,printSt);
		ProgramTree* printStr=new ProgramTree(currentIndent,str);
		string rmquotes=$2;
		rmquotes=rmquotes.substr(1,rmquotes.size()-2);
		printStr->setStringValue(rmquotes);
		print->addChild(printStr);
		statmentList.push_back(print);
	} |
	PRINT math ')'{
		ProgramTree* print=new ProgramTree(currentIndent,printSt);
		ProgramTree* printMathy=new ProgramTree(currentIndent,mathSt);
		printMathy->setStringValue($2);
		print->addChild(printMathy);
		statmentList.push_back(print);
	}
returnSt:
	RETURNS math{
		ProgramTree* returnStuff=new ProgramTree(currentIndent,returnStatment);
		ProgramTree* mathy=new ProgramTree(currentIndent,mathSt);
		mathy->setStringValue($2);
		returnStuff->addChild(mathy);
		statmentList.push_back(returnStuff);
	}
varassign:
	IDENT '=' math  { 
		ProgramTree* vAssign=new ProgramTree(currentIndent,varAssignSt);
		vAssign->setStringValue($1);
		ProgramTree* mathy=new ProgramTree(currentIndent,mathSt);
		mathy->setStringValue($3);
		vAssign->addChild(mathy);
		statmentList.push_back(vAssign);
	} 
ifstatment:
	IFID boolexpr ':'   { 
		ProgramTree* ifst=new ProgramTree(currentIndent,ifElSt);
		ProgramTree* boo=new ProgramTree(currentIndent,ifBool);
		ProgramTree* A;
		ProgramTree* B;
		boo->setStringValue(*$2.compare);
		if(*$2.firstVarname==""){
			A=new ProgramTree(currentIndent,intConst);
			A->setIntValue($2.firstInt);
		}
		else{
			A=new ProgramTree(currentIndent,variable);
			A->setStringValue(*$2.firstVarname);
		}

		if(*$2.secondVarname==""){
			B=new ProgramTree(currentIndent,intConst);
			B->setIntValue($2.secondInt);
		}
		else{
			B=new ProgramTree(currentIndent,variable);
			B->setStringValue(*$2.secondVarname);
		}

		boo->addChild(A);
		boo->addChild(B);
		ifst->addChild(boo);
		statmentList.push_back(ifst);
	} 
//will need to deal with int and variable comparison
boolexpr:
	INT BOOLSYMBOL INT {
		string emp="";
		$$.firstInt=$1;
		$$.firstVarname=&emp;
		string compSt=$2;
		$$.compare=&compSt;
		$$.secondInt=$3;
		$$.secondVarname=&emp;
		}
		|
		INT BOOLSYMBOL IDENT {
		string emp="";
		string identA=$3;
		$$.firstInt=$1;
		$$.firstVarname=&emp;
		string compSt=$2;
		$$.compare=&compSt;
		$$.secondInt=0;
		$$.secondVarname=&identA;
		}
		|
		IDENT BOOLSYMBOL INT {
		string emp="";
		string identA=$1;
		$$.firstInt=0;
		$$.firstVarname=&identA;
		string compSt=$2;
		$$.compare=&compSt;
		$$.secondInt=$3;
		$$.secondVarname=&emp;
		}
		|
		IDENT BOOLSYMBOL IDENT {
		string identA=$1;
		string identB=$3;
		$$.firstInt=0;
		$$.firstVarname=&identA;
		string compSt=$2;
		$$.compare=&compSt;
		$$.secondInt=0;
		$$.secondVarname=&identB;
		}
		
%%

void lnumber(){
	cout<<"LINES: "<<linenumber<<endl;
}
void yyerror(const char *s) {
	cout << "error linenumber"<<linenumber<<" " << s << endl;
}
vector<ProgramTree*> getStatmentList(){
	return statmentList;
}