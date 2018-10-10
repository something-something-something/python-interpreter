using namespace std;
#include <stdlib.h>
#include <string>
#include <vector>
#include "EvalMachine.hpp"
#include <iostream>


Variable::Variable(string n,int v,ProgramTree* s){
	name=n;
	value=v;
	scope=s;
}
void Variable::setValue(int v){
	value=v;
}
int Variable::getValue(){
	return value;
}
string Variable::getName(){
	return name;
}
ProgramTree* Variable::getScope(){
	return scope;
}
bool checker(char c,char min,char max){
	if( (c >= min) && (c<=max) ){
		return true;
	}
	else{
		return false;
	}
}

EvalMachine::EvalMachine(vector<Variable> v){
	//tree=ptree;
	variables=v;
}
int EvalMachine::evaluate(ProgramTree *current){
	switch(current->getType()){
		case varAssignSt: {
			int value=evaluate(current->getChild(0));
			assignVariable(current->getStringValue(),value,current->getScope());
		break;
		}
		case ifElSt: {
			ProgramTree* boolTest;
			for(int i=0;i<current->getChildren().size();i++){
				if(current->getChild(i)->getType()==ifBool){
					boolTest=current->getChild(i);
				}
			}
			int truth=evaluate(boolTest);
			if(truth==1){
				ProgramTree* ifSt;
				for(int i=0;i<current->getChildren().size();i++){
					if(current->getChild(i)->getType()==ifStatments){
						ifSt=current->getChild(i);
					}
				}
				evaluate(ifSt);
			}
		break;
		}
		case ifBool: {
			
			int a=evaluate(current->getChild(0));
			int b=evaluate(current->getChild(1));

			if(current->getStringValue()=="=="){
				return a==b;
			}
			else if(current->getStringValue()=="<="){
				return a<=b;
			}
			else if(current->getStringValue()==">="){
				return a>=b;
			}
			else if(current->getStringValue()=="<"){
				return a<b;
			}
			else if(current->getStringValue()==">"){
				return a>b;
			}
		break;
		}
		case variable: {
			return getVariableValue(current->getStringValue(),current->getScope());
		break;
		}
		case intConst: {
			return current->getIntValue();
		break;
		}
		case mathSt:{
			

			//TODO use postfix
			//NOT ACCURATE FOR ORDER OF OPERATIONS
			vector<MathComponent> mathVec;
			string mathStr=current->getStringValue();
			string currStr="";
			bool onIdentifier=false;
			bool onInt=false;
			for(int i=0;i<mathStr.length();i++){
				
				if(!onIdentifier&&!onInt){
					if(checker(mathStr[i],'a','z')||checker(mathStr[i],'A','Z')){
						onIdentifier=true;
					}
					else if(checker(mathStr[i],'0','9')){
						onInt=true;
					}
				}
				if(onIdentifier||onInt){
					if(mathStr[i]=='-'||mathStr[i]=='+'||mathStr[i]=='/'||mathStr[i]=='*'){
						if(onIdentifier){
							mathVec.push_back({currStr,0,' '});
							currStr="";
							onIdentifier=false;
							onInt=false;
						}
						else if(onInt){
							mathVec.push_back({"",atoi(currStr.c_str()),' '});
							currStr="";
							onIdentifier=false;
							onInt=false;
						}
						mathVec.push_back({"",0,mathStr[i]});
					}
					else{
						currStr+=mathStr[i];
					}
				}
				
			}
			
			if(onIdentifier){
				mathVec.push_back({currStr,0,' '});
				currStr="";
				onIdentifier=false;
				onInt=false;
			}
			else if(onInt){
				mathVec.push_back({"",atoi(currStr.c_str()),' '});
				currStr="";
				onIdentifier=false;
				onInt=false;
			}
			

			//do calculations
			int val=evaluateMathComponent(mathVec[0],current->getScope());

			
			if(mathVec.size()>2){
				for(int i=2;i<mathVec.size();i+=2){
					if(mathVec[i-1].symbol=='-'){
						val=val-evaluateMathComponent(mathVec[i],current->getScope());
					}
					else if(mathVec[i-1].symbol=='+'){
						val=val+evaluateMathComponent(mathVec[i],current->getScope());
					}
					else if(mathVec[i-1].symbol=='/'){
						val=val/evaluateMathComponent(mathVec[i],current->getScope());
					}
					else if(mathVec[i-1].symbol=='*'){
						val=val*evaluateMathComponent(mathVec[i],current->getScope());
					}

				}
			}
			return val;





		break;
		}
		case ifStatments: {
			for(int i=0;i<current->getChildren().size();i++){
				evaluate(current->getChild(i));
			}
		break;
		}
		case elseStatments:{
		//not currently implimented
		break;
		}
		case printSt: {
			for(int i=0;i<current->getChildren().size();i++){
				if(current->getChild(i)->getType()==str){
					cout<<current->getChild(i)->getStringValue();
				}
				else{
					cout<<evaluate(current->getChild(i));
				}
			}
			cout<<endl;
		break;
		}
		case str: {
		//not needed right now
		break;
		}
		case program:{ 
		for(int i=0;i<current->getChildren().size();i++){
				evaluate(current->getChild(i));
		}
		break;
		}
		
	}
	return 0;
}






int EvalMachine::evaluateMathComponent(MathComponent m,ProgramTree* scope){
	
	if(m.name!=""){
		//is variable
		
		return getVariableValue(m.name,scope);
	}
	else {
		//
			
		return m.value;
	}
	return 0;
}

void EvalMachine::assignVariable(string name,int value,ProgramTree* scope){
	bool found=false;
	for(int i=0;i<variables.size();i++){
		if(variables[i].getName()==name&&variables[i].getScope()==scope){
			variables[i].setValue(value);
			found=true;
		}
	}
	if(!found){
		variables.push_back(Variable(name,value,scope));
	}
}
int EvalMachine::getVariableValue(string name,ProgramTree* scope){
	bool found=false;
	for(int i=0;i<variables.size();i++){
		if(variables[i].getName()==name&&variables[i].getScope()==scope){
			return variables[i].getValue();
			
		}
	}
	if(!found){
		return 0;
	}
}


