
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
vector<char*> vecOfBisonStrings;
void createIfStatment(string firstVarname,
		int firstInt,
		string compare,
		string secondVarname,
		int secondInt);

%}

%union{
	char* s;
	int intv;
	char charv;
	struct BoolStruct{
		const char* firstVarname;
		int firstInt;
		const char* compare;
		const char* secondVarname;
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
%token <s> COMMENT
%token <s> ENDL
%token <intv> INDENT
%token <intv> PRINT
%token <intv> ELSE
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
	 returnSt|
	 commentstatment| else
	;

commentstatment:
	COMMENT ;
functiondef:
	FUNCDEF IDENT '(' ')' ':'{
		ProgramTree* func=new ProgramTree(currentIndent,functionDecleration);
		func->setStringValue($2);
		statmentList.push_back(func);
	}


mathstatment:
	math {
		ProgramTree* mathy=new ProgramTree(currentIndent,mathSt);
		mathy->setStringValue($1);
		statmentList.push_back(mathy);
		}
math:
	INT  {string i=to_string($1);
		$$=strdup(i.c_str());
		vecOfBisonStrings.push_back($$);
		}
	| 
	IDENT {$$=$1;}

	|
	IDENT '(' ')' {
		string funcall=$1;
		funcall=funcall+"()";
		$$=strdup(funcall.c_str());
		vecOfBisonStrings.push_back($$);
	}

	|INT MATHSYMBOL math {
		string mathSt=to_string($1);
		mathSt+=$2;
		mathSt+=$3;
		$$=strdup(mathSt.c_str());
		vecOfBisonStrings.push_back($$);
		}
		
	|
	IDENT MATHSYMBOL math {
		string mathSt=$1;
		mathSt+=$2;
		mathSt+=$3;
		$$=strdup(mathSt.c_str());
		vecOfBisonStrings.push_back($$);
		}
	|
	IDENT '(' ')' MATHSYMBOL math {
		string funcall=$1;
		funcall=funcall+"()";
		char* funCallCString=strdup(funcall.c_str());
		vecOfBisonStrings.push_back(funCallCString);
		string mathSt=funCallCString;
		mathSt+=$4;
		mathSt+=$5;
		$$=strdup(mathSt.c_str());
		vecOfBisonStrings.push_back($$);
		}

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
	}|
	PRINT math ',' STRING ')'{
		ProgramTree* print=new ProgramTree(currentIndent,printSt);

		ProgramTree* printMathy=new ProgramTree(currentIndent,mathSt);
		printMathy->setStringValue($2);
		print->addChild(printMathy);

		ProgramTree* printStr=new ProgramTree(currentIndent,str);
		string rmquotes=$4;
		rmquotes=rmquotes.substr(1,rmquotes.size()-2);
		printStr->setStringValue(rmquotes);
		print->addChild(printStr);

		statmentList.push_back(print);
	}|
	PRINT math ',' math ')'{
		ProgramTree* print=new ProgramTree(currentIndent,printSt);
		
		ProgramTree* printMathy=new ProgramTree(currentIndent,mathSt);
		printMathy->setStringValue($2);
		print->addChild(printMathy);
		
		ProgramTree* printMathy2=new ProgramTree(currentIndent,mathSt);
		printMathy2->setStringValue($4);
		print->addChild(printMathy2);

		statmentList.push_back(print);
	}|
	PRINT STRING ',' STRING ')'{
		ProgramTree* print=new ProgramTree(currentIndent,printSt);
		
		ProgramTree* printStr=new ProgramTree(currentIndent,str);
		string rmquotes=$2;
		rmquotes=rmquotes.substr(1,rmquotes.size()-2);
		printStr->setStringValue(rmquotes);
		print->addChild(printStr);
		
		ProgramTree* printStr2=new ProgramTree(currentIndent,str);
		rmquotes=$4;
		rmquotes=rmquotes.substr(1,rmquotes.size()-2);
		printStr2->setStringValue(rmquotes);
		print->addChild(printStr2);
		
		statmentList.push_back(print);
	}|
	PRINT STRING ',' math ')'{
		ProgramTree* print=new ProgramTree(currentIndent,printSt);

		ProgramTree* printStr=new ProgramTree(currentIndent,str);
		string rmquotes=$2;
		rmquotes=rmquotes.substr(1,rmquotes.size()-2);
		printStr->setStringValue(rmquotes);
		print->addChild(printStr);

		ProgramTree* printMathy=new ProgramTree(currentIndent,mathSt);
		printMathy->setStringValue($4);
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
		string firstVarname=$2.firstVarname;
		int firstInt=$2.firstInt;
		string compare=$2.compare;
		string secondVarname =$2.secondVarname;
		int secondInt=$2.secondInt;
		createIfStatment( firstVarname,
		firstInt,
		compare,
		secondVarname,
		secondInt);
	} |
	IFID '(' boolexpr ')' ':'   { 
		string firstVarname=$3.firstVarname;
		int firstInt=$3.firstInt;
		string compare=$3.compare;
		string secondVarname =$3.secondVarname;
		int secondInt=$3.secondInt;
		createIfStatment( firstVarname,
		firstInt,
		compare,
		secondVarname,
		secondInt);
	} 

//will need to deal with int and variable comparison
boolexpr:
	INT BOOLSYMBOL INT {
		$$.firstInt=$1;
		$$.firstVarname="";
		$$.compare=$2;
		$$.secondInt=$3;
		$$.secondVarname="";
		}
		|
		INT BOOLSYMBOL IDENT {
		string emp="";
		string identA=$3;
		$$.firstInt=$1;
		$$.firstVarname="";
		$$.compare=$2;
		$$.secondInt=0;
		$$.secondVarname=$3;
		}
		|
		IDENT BOOLSYMBOL INT {
		$$.firstInt=0;
		$$.firstVarname=$1;
		$$.compare=$2;
		$$.secondInt=$3;
		$$.secondVarname="";
		}
		|
		IDENT BOOLSYMBOL IDENT {
		$$.firstInt=0;
		$$.firstVarname=$1;
		$$.compare=$2;
		$$.secondInt=0;
		$$.secondVarname=$3;
		}
else:
	ELSE {
		ProgramTree* elseStatment=new ProgramTree(currentIndent,elseStatments);
		statmentList.push_back(elseStatment);
 }
		
%%

void createIfStatment(string firstVarname,int firstInt,string compare,	string secondVarname,int secondInt){
	ProgramTree* ifst=new ProgramTree(currentIndent,ifElSt);
	ProgramTree* boo=new ProgramTree(currentIndent,ifBool);
	ProgramTree* A;
	ProgramTree* B;
	boo->setStringValue(compare);
	if(firstVarname==""){
		A=new ProgramTree(currentIndent,intConst);
		A->setIntValue(firstInt);
	}
	else{
		A=new ProgramTree(currentIndent,variable);
		A->setStringValue(firstVarname);
	}

	if(secondVarname==""){
		B=new ProgramTree(currentIndent,intConst);
		B->setIntValue(secondInt);
	}
	else{
		B=new ProgramTree(currentIndent,variable);
		B->setStringValue(secondVarname);
	}

	boo->addChild(A);
	boo->addChild(B);
	ifst->addChild(boo);
	statmentList.push_back(ifst);

}
void lnumber(){
	cout<<"LINES: "<<linenumber<<endl;
}
void yyerror(const char *s) {
	cout << "error linenumber"<<linenumber<<" " << s << endl;
}
vector<ProgramTree*> getStatmentList(){
	return statmentList;
}
void delstringsbison(){
	for(int i=0;i<vecOfBisonStrings.size();i++){
		free(vecOfBisonStrings[i]);
	}
}