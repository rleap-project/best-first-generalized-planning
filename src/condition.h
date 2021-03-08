#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "common.h"
#include "variable.h"
#include "state.h"
#include "expression.h"

class Condition : public Expression{
public:
	// Non-parameterized actions
	Condition( const string &name = "", Variable *lhs = 0, Variable *rhs = 0 ) : Expression( name, lhs, rhs ){}

	virtual ~Condition(){}
	
	virtual bool evalCondition( const State *s ) const{
		return true;
	}

	virtual string toString( bool titled = true ) const{
		string ret = "";
		if( titled ){
			ret += "[CONDITION]: ";
		}
		ret += "( " + _lhs->toString() + " " + _name + " " + _rhs->toString() + " )";
		if( titled ) ret+= "\n";
		return ret;
	}
};

class GreaterEqual : public Condition{
public:
	GreaterEqual( Variable *lhs, Variable *rhs ) : Condition( ">=", lhs, rhs ) {}
	
	virtual ~GreaterEqual(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs >= rhs;
	}

}; 

class LesserEqual : public Condition{
public:
	LesserEqual( Variable *lhs, Variable *rhs ) : Condition( "<=", lhs, rhs ) {}
	
	virtual ~LesserEqual(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs <= rhs;
	}

}; 
class Greater : public Condition{
public:
	Greater( Variable *lhs, Variable *rhs ) : Condition( ">", lhs, rhs ) {}
	
	virtual ~Greater(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs > rhs;
	}

}; 

class Lesser : public Condition{
public:
	Lesser( Variable *lhs, Variable *rhs ) : Condition( "<", lhs, rhs ) {}
	
	virtual ~Lesser(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs < rhs;
	}

}; 

class Equals : public Condition{
public:
	Equals( Variable *lhs, Variable *rhs ) : Condition( "=", lhs, rhs ) {}
	
	virtual ~Equals(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs == rhs;
	}

}; 

class Add : public Condition{
public:
	Add( Variable *lhs, Variable *rhs, int max_val = MAX_VAL ) : Condition( "+", lhs, rhs ){
		_max_val = max_val;
	}
	
	virtual ~Add(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs + rhs < _max_val;
	}

private:
	int _max_val;
}; 

class Subtract : public Condition{
public:
	Subtract( Variable *lhs, Variable *rhs ) : Condition( "-", lhs, rhs ) {}
	
	virtual ~Subtract(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs >= rhs;
	}

}; 

class Different : public Condition{
public:
	Different( Variable *lhs, Variable *rhs ) : Condition( "!=", lhs, rhs ) {}
	
	virtual ~Different(){}
	
	virtual bool evalCondition( const State *s ) const{
		int lhs = getLHS( s );
		int rhs = getRHS( s );
		return lhs != rhs;
	}

};

#endif
