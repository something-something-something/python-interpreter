using namespace std;
#include <stdlib.h>
#include <string>
#include <vector>
#include <stack>
#include <queue>
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
int getPriority(MathComponent m){
	if(m.symbol=='+'||m.symbol=='-'){
		return 1;
	}
	else{
		return 2;
	}
}

bool checker(char c,char min,char max){
	if( (c >= min) && (c<=max) ){
		return true;
	}
	else{
		return false;
	}
}
Function::Function(string n,ProgramTree* loc,ProgramTree* s){
	name=n;
	location=loc;
	scope=s;
}
string Function::getName(){
	return name;
}
ProgramTree* Function::getScope(){
	return scope;
}
ProgramTree* Function::getLocation(){
	return location;
}
void Function::setLocation(ProgramTree* loc){
	location=loc;
}



EvalMachine::EvalMachine(vector<Variable> v,vector<Function> f){
	//tree=ptree;
	variables=v;
	functions=f;
}
int EvalMachine::evaluate(ProgramTree *current){
	switch(current->getType()){
		case varAssignSt: {
			int value=evaluate(current->getChild(0));
			assignVariable(current->getStringValue(),value,current->getScope());
		break;
		}
		case returnStatment: {
			return evaluate(current->getChild(0));
		break;
		}
		case functionDecleration: {
			ProgramTree* location=current;
			assignFunction(current->getStringValue(),location,current->getScope());
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
			else{
				ProgramTree* elSt;
				bool elseExists=false;
				for(int i=0;i<current->getChildren().size();i++){
					if(current->getChild(i)->getType()==elseStatments){
						elSt=current->getChild(i);
						elseExists=true;
					}
				}
				if(elseExists){
					evaluate(elSt);
				}
			}
		break;
		}
		case ifBool: {
			
			int a=evaluate(current->getChild(0));
			int b=evaluate(current->getChild(1));

			if(current->getStringValue()=="=="){
				return a==b;
			}
			else if(current->getStringValue()=="!="){
				return a!=b;
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
			
			//cout<< current->getStringValue()<<endl;
			vector<MathComponent> mathVec;
			string mathStr=current->getStringValue();
			string currStr="";
			bool onIdentifier=false;
			bool onInt=false;
			bool onFunc=false;
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
						if(onIdentifier&&!onFunc){
							mathVec.push_back({currStr,0,' ',false});
							currStr="";
							onIdentifier=false;
							onInt=false;
							onFunc=false;
						}
						else if(onIdentifier&&onFunc){
							
							mathVec.push_back({currStr,0,' ',true});
							currStr="";
							onIdentifier=false;
							onInt=false;
							onFunc=false;
						}
						else if(onInt){
							mathVec.push_back({"",atoi(currStr.c_str()),' ',false});
							currStr="";
							onIdentifier=false;
							onInt=false;
							onFunc=false;
						}
						mathVec.push_back({"",0,mathStr[i]});
					}
					else if(mathStr[i]=='('&&onIdentifier&&!onFunc){
						onFunc=true;
							
					}
					else if(mathStr[i]==')'&&onIdentifier&&onFunc){
						//
						
					}
					else{
						currStr+=mathStr[i];
					}
				}
				
			}
			if(onIdentifier&&!onFunc){
				mathVec.push_back({currStr,0,' ',false});
				currStr="";
				onIdentifier=false;
				onInt=false;
				onFunc=false;
			}
			else if(onIdentifier&&onFunc){
				
				mathVec.push_back({currStr,0,' ',true});
				currStr="";
				onIdentifier=false;
				onInt=false;
				onFunc=false;
			}
			else if(onInt){
				mathVec.push_back({"",atoi(currStr.c_str()),' ',false});
				currStr="";
				onIdentifier=false;
				onInt=false;
				onFunc=false;
			}
			//cout<<"postfix start"<<endl;

			stack <MathComponent> operators;
			queue <MathComponent> output;
			for(int i=0;i<mathVec.size();i++){
				if(mathVec[i].symbol==' '){
					output.push(mathVec[i]);
				}
				else{
					while(operators.size()>0&&
					getPriority(operators.top())>=
					getPriority(mathVec[i])
					){
						output.push(operators.top());
						operators.pop();
					}
					operators.push(mathVec[i]);
				}
			}
			while(operators.size()>0){
				output.push(operators.top());
				operators.pop();
			}
			//cout<<"postfix output stack done Stack is size"<<output.size()<<endl;
		
			MathComponent a;
			MathComponent b;
			stack<MathComponent> answer;
			while(output.size()>0){
				//cout<<output.front().symbol<<endl;
				if(output.front().symbol==' '){
					//cout<<"pushing nonoperator to answer"<<endl;
					answer.push(output.front());
					output.pop();
				}
				else{
					//cout<<"IS operator"<<endl;
					char sy=output.front().symbol;
					//cout<<"getting b from answer"<<endl;
					b=answer.top();
					answer.pop();
					//cout<<"getting a from answer"<<endl;
					a=answer.top();
					answer.pop();
					if(sy=='+'){
						answer.push({"",
						evaluateMathComponent(a,current->getScope())
						+
						evaluateMathComponent(b,current->getScope())
						,' ',false});
					}
					else if(sy=='-'){
						answer.push({"",
						evaluateMathComponent(a,current->getScope())
						-
						evaluateMathComponent(b,current->getScope())
						,' ',false});
					}
					else if(sy=='*'){
						answer.push({"",
						evaluateMathComponent(a,current->getScope())
						*
						evaluateMathComponent(b,current->getScope())
						,' ',false});
					}
					else if(sy=='/'){
						answer.push({"",
						evaluateMathComponent(a,current->getScope())
						/
						evaluateMathComponent(b,current->getScope())
						,' ',false});	
					}
					output.pop();
				}
			}
			//cout<<"postfix end"<<endl;
			return evaluateMathComponent(answer.top(),current->getScope());




			//do calculations
			
			/*
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
			*/






		break;
		}
		case ifStatments: {
			for(int i=0;i<current->getChildren().size();i++){
				evaluate(current->getChild(i));
			}
		break;
		}
		case elseStatments:{
			for(int i=0;i<current->getChildren().size();i++){
				evaluate(current->getChild(i));
			}
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
		case functionStatments:{ 
			for(int i=0;i<current->getChildren().size();i++){
				if(current->getChild(i)->getType()==returnStatment){
					
					return evaluate(current->getChild(i));
				}
				else{
					evaluate(current->getChild(i));
				}
			}
		break;
		}
		
	}
	return 0;
}






int EvalMachine::evaluateMathComponent(MathComponent m,ProgramTree* scope){
	
	if(m.name!=""){
		//is variable
		if(m.isFunc){
			
			return evaluate(getFunctionLocation(m.name,scope)->getChild(0));
		}
		else{
			
			return getVariableValue(m.name,scope);
		}
		
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
void EvalMachine::assignFunction(string name,ProgramTree* loc,ProgramTree* scope){
	bool found=false;
	for(int i=0;i<functions.size();i++){
		if(functions[i].getName()==name&&functions[i].getScope()==scope){
			functions[i].setLocation(loc);
			found=true;
		}
	}
	if(!found){
		functions.push_back(Function(name,loc,scope));
	}
}
int EvalMachine::getVariableValue(string name,ProgramTree* scope){
	bool found=false;
	for(int i=0;i<variables.size();i++){
		if(variables[i].getName()==name&&variables[i].getScope()==scope){
			return variables[i].getValue();
			
		}
	}
	string exceptionreason="variable "+name+" not initialized";
	throw exceptionreason;
	return 0;
}
ProgramTree* EvalMachine::getFunctionLocation(string name,ProgramTree* scope){
	bool found=false;
	for(int i=0;i<functions.size();i++){
		if(functions[i].getName()==name&&functions[i].getScope()==scope){
			return functions[i].getLocation();
		}
	}
	string exceptionreason="function "+name+" not defined";
	throw exceptionreason;
	return 0;
}

