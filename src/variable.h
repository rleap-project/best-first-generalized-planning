#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "common.h"

// Counters and Pointers represented as variables, 
// where only the latters have access to registers
class Variable{
public:
	Variable( const string &name = "", const string &vtype = "constant", int id = 0, bool am = false ){
		_name = name;
		_vtype = vtype;
		_id = id;
		_access_memory = am;
	}
	
	~Variable(){
	}
	
	void setName( const string &name = "" ){
		_name = name;
	}

	void setVType( const string &vtype = "constant" ){
		_vtype = vtype;
	}
	
	void setID( int id ) {
		_id = id;
	}
	
	void setAccessMemory( bool am ){
		_access_memory = am;
	}
	
	string getName() const{
		return _name;
	}
	
	string getVType() const{
		return _vtype;
	}
	
	int getID() const{
		return _id;
	}
	
	bool accessMemory() const{
		return _access_memory;
	}
	
	string toString( bool info = false ) const{
		string ret = "";
		if( info )
			ret = "[VARIABLE]: ";
		ret += (_access_memory?"*":"") + _name;
		if( info ){
			ret += " (" + _vtype +") id: " + to_string( _id );
		}
		return ret;
	}
	
private:
	string _name;
	string _vtype;
	int _id;
	bool _access_memory;
};

#endif