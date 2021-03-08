#ifndef __STATE_DESCRIPTOR_H__
#define __STATE_DESCRIPTOR_H__

#include "common.h"

class StateDescriptor{
public:
	StateDescriptor(){
		_counters = 0;
		_pointers = 0;
		_const_pointers = 0;
		_registers = 0;
		
		addStateVariable( "zf", "pointer" ); // zero-flag
		addStateVariable( "cf", "pointer" ); // carry-flag
	}

	virtual ~StateDescriptor(){
		_var_id.clear();
		_var_types.clear();
	}
	
	int getID( const string &state_var = "" ){
		map< string, int >::iterator it = _var_id.find( state_var );
		if( it == _var_id.end() ) 
			return -1; // state var does not exist
		return it->second;
	}
	
	string getVType( const string &state_var = "" ){
		map< string, string >::iterator it = _var_types.find( state_var );
		if( it == _var_types.end() )
			return "";
		return it->second;
	}

	bool addStateVariable( const string &state_var, const string &type_var ){
		// State variables must be counters, pointers or registers
		if( type_var != "counter" and type_var != "pointer" and type_var != "const-pointer" and type_var != "register" )
			return false;

		int id = getID( state_var );
		
		// State variable already exists
		if( -1 != id )
			return false;
		
		if( type_var == "counter" ){
			_var_id[ state_var ] = _counters++;
		}
		else if( type_var == "pointer" ){
			_var_id[ state_var ] = _pointers++;
		}
		else if( type_var == "const-pointer" ){
			_var_id[ state_var ] = _const_pointers++;
		}
		else if( type_var == "register" ){
			_var_id[ state_var ] = _registers++;
		}

		_var_types[ state_var ] = type_var;

		return true;
	}
	
	vector< string > getVars() const{
		vector< string > ret;
		for( auto it = _var_id.begin(); it != _var_id.end(); it++ )
			ret.push_back( it->first );
		return ret;
	}
	
	int getNumCounters() const{
		return _counters;
	}
	
	int getNumPointers() const{
		return _pointers;
	}
	
	int getNumConstPointers() const{
		return _const_pointers;
	}
		
	int getNumRegisters() const{
		return _registers;
	}
	
	//[ToDo]? We consider the memory is shared
	int getMemorySize( int num_objects ) const{
		return (_registers * num_objects );
	}

	int getStateSize( int num_objects ) const{
		return ( _counters + _pointers ) + ( _registers * num_objects );
	}
	
	string toString(){
		string ret = "[STATE DESCRIPTOR]:\n";
		ret += "#Counters: " + to_string( _counters ) + "\n";
		ret += "#Pointers: " + to_string( _pointers ) + "\n";
		ret += "#Const-Pointers: " + to_string( _const_pointers ) + "\n";
		ret += "#Registers: " + to_string( _registers ) + "\n";

		ret += "State Variables:\n";
		
		for( auto it = _var_id.begin(); it != _var_id.end(); it++ ){
			ret += (it->first) + " (" + _var_types[ (it->first) ] + ") id: " + to_string(it->second) + "\n";
		}
		
		return ret;
	}
private:
	map< string, int > _var_id;
	map< string, string > _var_types;
	int _counters;
	int _pointers;
	int _const_pointers;
	int _registers;
}; 

#endif
