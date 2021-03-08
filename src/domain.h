#ifndef __DOMAIN_H__
#define __DOMAIN_H__

#include "common.h"
#include "state_descriptor.h"
#include "action.h"

class Domain{
public:
	Domain( const string &name = "" ){
		_name = name;
		//_sd = NULL;
	}
		
	~Domain(){
		if( _sd ) delete _sd;
		for( int i = 0; i < int( _actions.size() ); i++ ){
			if( _actions[ i ] )
				delete _actions[ i ];
		}
	}

	void setName( const string &name = "" ){
		_name = name;
	}

	void setStateDescriptor( StateDescriptor *sd ){
		_sd = sd;
	}
	
	void addAction( Action *a ){
		_actions.push_back( a );
	}
	
	StateDescriptor* getStateDescriptor(){
		return _sd;
	}
	
	vector< Action* > getActions() const{
		return _actions;
	}
	
	Action* getAction( int id ){
		return _actions[ id ];
	}
	
	Action* getAction( const string &name = "" ){
		for( int id = 0; id < int( _actions.size()); id++ ){
			if( _actions[ id ]->getName() == name )
				return _actions[ id ];
		}
		return NULL;
	}
	
	int getNumActions() const{
		return _actions.size();
	}
	
	string getName() const{
		return _name;
	}
	
	string toString() const{
		string ret = "[DOMAIN]: " + _name + "\n";
		if( _sd )
			ret += "\n" + _sd->toString();
		for( int i = 0; i < int( _actions.size() ); i++ ){
			ret += "\n" + _actions[ i ]->toString();
		}
		return ret;
	}
	
private:
	string _name;
	StateDescriptor *_sd;
	vector< Action* > _actions;
}; 

#endif
