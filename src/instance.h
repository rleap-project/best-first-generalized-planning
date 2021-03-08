#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "common.h"
#include "state.h"
#include "condition.h"

class Instance{
public:
	Instance( const string &name = "" ){
		_name = name;
		_init = NULL;
	}
	
	~Instance(){
		if( _init ) delete _init;

		for( int i = 0; i < int( _goal.size() ); i++ ){
			if( _goal[ i ] )
				delete _goal[ i ];
		}
		_goal.clear();
	}
	
	void setName( const string &name = "" ){
		_name = name;
	}
	
	void setInitialState( State *init ){
		_init = init;
	}
	
	void setGoalCondition( vector< Condition* > &goal ){
		_goal = goal;
	}
	
	void addCondition( Condition* cond ){
		_goal.push_back( cond );
	}
	
	string getName() const{
		return _name;
	}
	
	State* getInitialState(){
		return _init;
	}
	
	vector< Condition* > getGoalCondition(){
		return _goal;
	}
	
	Condition* getCondition( int id ){
		return _goal[ id ];
	}

	// Full info printing
	string toString() const{
		string ret = "[INSTANCE]: " + _name + "\n";
		
		ret += "\nINIT:\n";
		if( _init )
			ret += _init->toString();
		ret += "\nGOAL:\n";
		for( int i = 0; i < int( _goal.size() ); i++ )
			ret += _goal[i]->toString();
		return ret;
	}

	// Pretty printing
	string toString( StateDescriptor *sd ){
		string ret = "[INSTANCE]: " + _name + "\n";
		
		ret += "\nINIT:\n";
		if( _init )
			ret += _init->toString( sd );
		ret += "\nGOAL:\n";
		for( int i = 0; i < int( _goal.size() ); i++ )
			ret += _goal[i]->toString( false ) + "\n";
		return ret;
	}
	
private:
	string _name;
	State *_init;
	vector< Condition* > _goal;
};

#endif
