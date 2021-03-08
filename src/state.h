#ifndef __STATE_H__
#define __STATE_H__

#include "common.h"
#include "state_descriptor.h"

class State{
public:
	State( int ncounters = 0, int npointers = 0, int nconst_pointers = 0, int nregisters = 0  ){
		_counters = vector< int >( ncounters, 0 );
		_pointers = vector< int >( npointers, 0 );
		_const_pointers = vector< int >( nconst_pointers, 0 );
		_registers = vector< int >( nregisters, -1 );
	}

	State( const vector<vector<int> > &v ){
		_counters = v[0];
		_pointers = v[1];
		_const_pointers = v[2];
		_registers = v[3];
	}
	
	State( const State &s ){
		vector<vector<int> > vars = s.getVars();
		_counters = vars[0];
		_pointers = vars[1];
		_const_pointers = vars[2];
		_registers = vars[3];
	}
	
	State( const State *s ){
		vector<vector<int> > vars = s->getVars();
		_counters = vars[0];
		_pointers = vars[1];
		_const_pointers = vars[2];
		_registers = vars[3];
	}
	
	State* copy(){
		return new State( this );
	}
	
	void setCounter( int idx, int value ){
		_counters[ idx ] = value;
	}
	
	void setPointer( int idx, int value ){
		_pointers[ idx ] = value;
	}
	
	void setRegister( int idx, int value ){
		_registers[ idx ] = value;
	}
	
	void setConstPointer( int idx, int value ){
		_const_pointers[ idx ] = value;
	}
	
	void setVar( int idx, int value, const string &vtype = "" ){
		if( vtype == "counter" )
			setCounter( idx, value );
		else if( vtype == "pointer" )
			setPointer( idx, value );
		else if( vtype == "const-pointer" )
			setConstPointer( idx, value );
		else if( vtype == "register" )
			setRegister( idx, value );
	}
	
	int getCounter( int idx ) const{
		if( idx >= int( _counters.size() ) )
			return -1;
		return _counters[ idx ];
	}
	
	int getPointer( int idx ) const{
		if( idx >= int( _pointers.size() ) )
			return -1;
		return _pointers[ idx ];
	}
	
	int getRegister( int idx ) const{
		if( idx >= int( _registers.size() ) )
			return -1;
		return _registers[ idx ];
	}
	
	int getConstPointer( int idx ) const{
		if( idx >= int( _const_pointers.size() ) )
			return -1;
		return _const_pointers[ idx ];
	}
	
	vector< vector< int > > getVars() const{
		vector< vector< int > > vars;
		vars.push_back( _counters );
		vars.push_back( _pointers );
		vars.push_back( _const_pointers );
		vars.push_back( _registers );
		return vars;
	}
	
	vector< int > getCounters() const{
		return _counters;
	}

	vector< int > getPointers() const{
		return _pointers;
	}

	vector< int > getRegisters() const{
		return _registers;
	}
	
	vector< int > getConstPointers() const{
		return _const_pointers;
	}
	
	int getNumCounters() const{
		return int( _counters.size() );
	}
	
	int getNumPointers() const{
		return int( _pointers.size() );
	}
	
	int getNumRegisters() const{
		return int( _registers.size() );
	}
	
	int getNumConstPointers() const{
		return int( _const_pointers.size() );
	}
	
	int size() const{
		return getNumCounters() + getNumPointers() + getNumRegisters() + getNumConstPointers();
	}

	string toString() const{
		string ret = "[STATE]:\n";
		ret += "COUNTERS:";
		for( int i = 0; i < int( _counters.size() ); i++){
			ret += " " + to_string( _counters[i] );
		} 
		ret += "\nPOINTERS:";
		for( int i = 0; i < int( _pointers.size() ); i++){
			ret += " " + to_string( _pointers[i] );
		} 
		ret += "\nCONSTANT POINTERS:";
		for( int i = 0; i < int( _const_pointers.size() ); i++){
			ret += " " + to_string( _const_pointers[i] );
		} 
		ret += "\nREGISTERS:";
		for( int i = 0; i < int( _registers.size() ); i++){
			ret += " " + to_string( _registers[i] );
		} 
		ret += "\n";
		return ret;
		
	}
	
	string toString( StateDescriptor *sd ){
		string ret = "";
		vector< string > var_names = sd->getVars();
		for( int i = 0; i < int( var_names.size() ); i++ ){
			string vtype = sd->getVType( var_names[ i ] );
			int id = sd->getID( var_names[ i ] );

			string value = "";
			if( vtype == "counter" )
				value = to_string( _counters[ id ] );
			else if( vtype == "pointer" )
				value = to_string( _pointers[ id ] );
			else if( vtype == "const-pointer" )
				value = to_string( _const_pointers[ id ] );
			else if( vtype == "register" )
				value = to_string( _registers[ id ] );

			ret += "( " + var_names[ i ] + " = " + value + " )\n";
		}
		return ret;
	}

private:
	vector<int> _counters;
	vector<int> _pointers;
	vector<int> _const_pointers;
	vector<int> _registers;
};

#endif
