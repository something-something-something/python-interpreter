%option noyywrap

%{

using namespace std;
#include <iostream>
#include <stdlib.h>
#include <string>
#include "python.tab.h"
#include <vector>
vector<char*> vecOfStrings;
%}
%%
\n {
	
	yylval.s=strdup(yytext);
	vecOfStrings.push_back(yylval.s);

 return ENDL;}
[0-9]* {yylval.intv=atoi(yytext); return INT;}

\"[a-z A-Z0-9]*\" { 
	yylval.s=strdup(yytext);
	vecOfStrings.push_back(yylval.s);
	 return STRING;} /*obviously would use a diffrent regexp for string*/
if[ ]+ { 
	yylval.s=strdup(yytext);
	vecOfStrings.push_back(yylval.s);
	 return IFID;}
print\( {return PRINT;}
[\+\-\/\*] { 
	yylval.s=strdup(yytext);
	vecOfStrings.push_back(yylval.s); 
	return MATHSYMBOL;}
[a-zA-Z]* { 
	yylval.s=strdup(yytext);
	vecOfStrings.push_back(yylval.s);
	 return IDENT;}/*obviously would use a diffrent regexp for string*/
==|>|<|>=|<= { yylval.s=strdup(yytext); 
vecOfStrings.push_back(yylval.s); 
return BOOLSYMBOL;}
= {return '=';}
: {return ':';}
"(" {return '(';}
")" {return ')';}
^[ ]+ {yylval.intv=strlen(yytext);return INDENT;}
. ;
%%

void delstringsflex(){
	for(int i=0;i<vecOfStrings.size();i++){
		free(vecOfStrings[i]);
	}
	
}