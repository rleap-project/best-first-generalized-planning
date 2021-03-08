#ifndef __NODE_H__
#define __NODE_H__

#include "common.h"
#include "instruction.h"
#include "program.h"

class Node{
public:	
	Node( Program *p, const vector<int> &evaluations = vector<int>(1,INF) ){
		_program = p;
		_evaluations = evaluations;
	}
	/*Node( Instruction *ins, int line, int g = INF, const vector<int> &evaluations = vector<int>(1,INF), Node* ancestor = NULL ){
		_instruction = ins;
		_line = line;
		_g = g;
		_evaluations = evaluations;
		_ancestor = ancestor;
	}*/
	
	~Node(){
		if( _program )
			delete _program;
	}
	
	/*void setG( int g ){
		_g = g;
	}
	
	int g() const{
		return _g;
	}*/
	
	void setF( const vector<int> &eval ){
		_evaluations = eval;
	}
	
	vector< int > f() const{
		return _evaluations;
	}
	
	/*int getProgramLine(){
		return _line;
	}*/
	
	/*Instruction* getInstruction(){
		return _instruction;
	}*/
	
	Program* getProgram(){
		return _program;
	}
	
	/*Node* getAncestor(){
		return _ancestor;
	}*/
	
	/*Program* generateProgram( int program_lines ){
		Program *prog = new Program( program_lines );
		Node *current = this;
		
		while( current != NULL && current->getInstruction() != NULL ){
//cout << "CURRENT: " << current->toString() << endl;
			prog->setInstruction( current->getProgramLine(), current->getInstruction() );
			current = current->getAncestor();
		}
		
		return prog;
	}*/
	
	string toString(){
		//string ret = "[NODE]: g="+to_string(_g)+" pc="+to_string(_line)+"\n";
		string ret = "[NODE]:";
		for( int i = 0; i < int( _evaluations.size() ); i++ ){
			ret+=" f"+to_string(i+1)+"="+to_string(_evaluations[i]);
		}
		ret+="\n";
		if( _program )
			ret += _program->toString( false );
		//if( _instruction )
		//	ret += _instruction->toString();
		return ret;
	}
	
private:
	//int _g;
	vector< int > _evaluations;
	//int _line;
	//Instruction *_instruction;
	Program *_program;
	//Node *_ancestor;
};

#endif
