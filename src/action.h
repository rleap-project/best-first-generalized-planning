#ifndef __ACTION_H__
#define __ACTION_H__

#include "common.h"
#include "state.h"
#include "state_descriptor.h"

class Action{
public:
	Action( const string &name = "", const string &atype = "math" ) : _name( name ), _atype( atype ){
	}
	
	virtual ~Action(){
		for( int i = 0; i < int( _preconditions.size() ); i++ ){
			if( _preconditions[ i ] )
				delete _preconditions[ i ];
		}
		
		for( int i = 0; i < int( _effects.size() ); i++ ){
			if( _effects[ i ] )
				delete _effects[ i ]; 
		}
	}
	
	virtual bool isApplicable( const State *s ){
		for( int i = 0; i < int( _preconditions.size() ); i++ ){
			if( ! _preconditions[ i ]->evalCondition( s ) )
				return false;
		}
		return true;
	}
	
	virtual int apply( State *s ){
		vector< int > num_effects( _effects.size(), 0 );
		for( int i = 0; i < int( _effects.size() ); i++ ){
			num_effects[ i ] = ( _effects[ i ]->getEffect( s ) );
		}
		for( int i = 0; i < int( _effects.size() ); i++ ){
			_effects[ i ]->setEffect( s, num_effects[ i ] );
		}
		// Return the last effect
		if( int( num_effects.size() ) == 0 ) return 0; // 0 by default if no effects, e.g. CMP action
		return num_effects[ int(num_effects.size()) - 1 ];
	}
	
	virtual void addCondition( Condition *cond ){
		_preconditions.push_back( cond );
	}

	virtual void addOperation( Operation* op ){
		_effects.push_back( op );
	}

	virtual string getName() const{
		return _name;
	}
	
	virtual string getType() const{
		return _atype;
	}
	
	virtual string toString( bool full_info = true ) const{
		if( !full_info ) return _name+"\n";
		string ret = "[ACTION]: " + _name + "\n";
		ret += "PRECONDITIONS:\n";
		for( int i = 0; i < int( _preconditions.size() ); i++ ){
			ret += _preconditions[i]->toString( false ) + "\n";
		}
		ret += "EFFECTS:\n";
		for( int i = 0; i < int( _effects.size() ); i++ ){
			ret += _effects[i]->toString( false );
		}
		return ret;
	}
	
private:
	string _name;
	string _atype;
	vector< Condition* > _preconditions;
	vector< Operation* > _effects;
};

#endif
