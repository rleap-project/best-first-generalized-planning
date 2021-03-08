#ifndef __OPERATION_H__
#define __OPERATION_H__

#include "common.h"
#include "state.h"
#include "expression.h"

class Operation : public Expression{
public:
	// Non-parameterized actions
	Operation( const string &name = "", Variable *lhs = 0, Variable *rhs = 0 ) : Expression( name, lhs, rhs ) {}

	virtual ~Operation(){}
	
	virtual int update( State *s ) const{ 
		int effect = getEffect( s );
		setEffect( s, effect ); 
		return effect; 
	}
	
	virtual int getEffect( State *s ) const{ return 0; }
	
	virtual void setEffect( State *s, int val ) const{
		if( val >= 0 )
			setLeftVarNum( s, val );
	}

	virtual string toString( bool titled = true ) const{
		string ret = "";
		if( titled ){
			ret += "[OPERATION]: ";
		}
		ret += "( " + _lhs->toString() + " " + _name + " " + _rhs->toString() + " )\n";
		return ret;
	}
};

class AddAssign : public Operation{
public:
	AddAssign( Variable *lhs = 0, Variable *rhs = 0 ) : Operation( "+=", lhs, rhs ) {}
	
	virtual ~AddAssign(){}
	
	virtual int update( State *s ) const{
		int effect = getEffect( s );
		setEffect( s, effect );
		return effect;
	}
	
	virtual int getEffect( State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs + rhs;
	}

}; 

class SubtractAssign : public Operation{
public:
	SubtractAssign( Variable *lhs = 0, Variable *rhs = 0 ) : Operation( "-=", lhs, rhs ) {}
	
	virtual ~SubtractAssign(){}
	
	virtual int getEffect( State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs - rhs;
	}

}; 

class Compare : public Operation{
public:
	Compare( Variable *lhs = 0, Variable *rhs = 0 ) : Operation( "-", lhs, rhs ) {}
	
	virtual ~Compare(){}
	
	virtual int getEffect( State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		if( lhs < 0 or rhs < 0 )
			return -1; // [ENHANCEMENT] This should never happen, otherwise return an infinite
		return lhs - rhs;
	}
	
	virtual void setEffect( State *s, int val ) const{
		// no updates for cmp
	}
}; 

class Assign : public Operation{
public:
	Assign( Variable *lhs = 0, Variable *rhs = 0 ) : Operation( "=", lhs, rhs ) {}
	
	virtual ~Assign(){}
	
	virtual int getEffect( State *s ) const{
		//int lhs = getLHS( s );
		int rhs = getRHS( s );
		return rhs;
	}

}; 

#endif
