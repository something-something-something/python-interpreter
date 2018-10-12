#include "ProgramTree.hpp"
#include <stdlib.h>
#include <string>
#include <iostream>

ProgramTree* ProgramTree::getChild(int i){
	return children[i];
}
ProgramTree* ProgramTree::getParent(){
	return parent;
}

vector<ProgramTree*> ProgramTree::getChildren(){
	return children;
}

ProgramTree::ProgramTree(int i,ParseTreeType t){
	indent=i;
	treeType=t;
	intValue=0;
	stringValue="";
	parent=NULL;
	

}
string ProgramTree::getStringValue(){
	return stringValue;
}
int ProgramTree::getIntValue(){
	return intValue;
}
void ProgramTree::setIntValue(int i){
	intValue=i;
}
void ProgramTree::setParent(ProgramTree* p){
	parent=p;
	
}
void ProgramTree::addChild(ProgramTree* c){
	c->setParent(this);
	children.push_back(c);
	c->setParent(this);
	//cout<<"parent: "<<this->getType()<<"to "<<c->getType()<<endl;
	
}
void ProgramTree::setStringValue(string s){
	stringValue=s;
}
void ProgramTree::print(){
	string indentString="";
	indentString.append(indent,' ');
	cout<<indentString;
	string theType=to_string(treeType);
	switch(treeType){
		case varAssignSt: theType="varAssignSt";
		break;
		case ifElSt: theType="ifElSt";
		break;
		case ifBool: theType="ifBool";
		break;
		case variable: theType="variable";
		break;
		case intConst: theType="intConst";
		break;
		case mathSt: theType="mathSt";
		break;
		case ifStatments: theType="ifStatments";
		break;
		case elseStatments: theType="elseStatments";
		break;
		case printSt: theType="printSt";
		break;
		case str: theType="str";
		break;
		case program: theType="program";
		break;
		case functionDecleration: theType="functionDecleration";
		break;
		case functionStatments: theType="functionStatments";
		break;
		case returnStatment: theType="returnStatment";
		break;
	}

	cout<<"Type"<<": "<<theType;
	cout<<" INT V: "<<intValue;
	cout<<" String Value: "<<stringValue;
	cout<<" Indentation: "<<indent;
	cout<<" This: "<<this;
	cout<<" Parent: "<<parent;
	cout<<endl;

	if(children.size()>0){
		cout<<"!!children of "<<theType<<endl;
		for(int i=0;i<children.size();i++){
			children[i]->print();
		}
		cout<<"!!end children of "<<theType<<endl;
	}
}
ParseTreeType ProgramTree::getType(){
	return treeType;
}
int ProgramTree::getIndent(){
	return indent;
}
ProgramTree* ProgramTree::getScope(){
	/*ProgramTree* scope=this;
	cout<<scope<<endl;
	while(parent!=NULL && scope->getType()!=program){
		
		scope=parent;
	}
	return scope;*/
	if(parent==NULL||this->getType()==program){
		return this;
	}
	else if(parent->getType()==functionDecleration){
		return parent;
	}
	else{
		return parent->getScope();
	}

}
ProgramTree::~ProgramTree(){
	for(int i=0;i<children.size();i++){
			delete children[i];
		}
}