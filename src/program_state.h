#ifndef __PROGRAM_STATE_H__
#define __PROGRAM_STATE_H__

#include "common.h"
#include "state.h"

class ProgramState{
public:
	ProgramState(){
		_state = NULL;
		_line = 0;
	}
	
	~ProgramState(){
		if( _state ) 
			delete _state;
	}
	
	void setState( State *s ){
		_state = s;
	}
	
	void setLine( int l ){
		_line = l;
	}
	
	State* getState(){
		return _state;
	}
	
	int getLine(){
		return _line;
	}
	
	string toString(){
		string ret = "[PROGRAM STATE]:\nLINE:" + to_string( _line ) + "\n";
		if( _state )
			ret += _state->toString();
		return ret;
	}
	
private:
	State *_state;
	int _line;
};

#endif
