#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

class Expression{
public:
	Expression( const string &name = "", Variable *lhs = 0, Variable *rhs = 0 ){
		_name = name;
		_lhs = lhs;
		_rhs = rhs;
	}
	
	~Expression(){
		if( _lhs ) delete _lhs;
		if( _rhs ) delete _rhs;
	}
	virtual int update( State *s ) const{ return 0; }

	virtual string getName() const{
		return _name;
	}

	virtual string toString( bool titled = true ) const{
		string ret = "";
		if( titled ){
			ret += "[EXPRESSION]: ";
		}
		ret += "( " + _lhs->toString() + " " + _name + " " + _rhs->toString() + " )\n";
		return ret;
	}

	// Set the left-hand side variable of the expression
	virtual void setLHS( Variable *v ){
		_lhs = v;
	}
	
	// Set the right-hand side of the variable of the expression
	virtual void setRHS( Variable *v ){
		_rhs = v;
	}
	
	// Set the content of left-hand side variable
	virtual void setLeftVarNum( State *s, int number ) const{
		int id = _lhs->getID();
		string vtype = _lhs->getVType();
	
		if( vtype == "counter" )
			s->setCounter( id, number );
		else if( vtype == "pointer" ){
			if( _lhs->accessMemory() ){
				s->setRegister( s->getPointer( id ), number );
			}
			else{
				s->setPointer( id, number );
			}
		}
		else if( vtype == "register" )
			s->setRegister( id, number );
	}
	
	// Value of the variable in current state or constant number
    virtual int getVarNum( const State* s, Variable *v ) const{
		int id = v->getID();
		string vtype = v->getVType();

		if( vtype == "counter" )
			return s->getCounter( id );

		else if( vtype == "pointer" ){
			if( v->accessMemory() ){
				return s->getRegister( s->getPointer( id ) );
			}
			return s->getPointer( id );
		}	

		else if( vtype == "register" )
			return s->getRegister( id );
			
		else if( vtype == "const-pointer" ){
		    if( v->accessMemory() ) {
                return s->getRegister(s->getConstPointer(id));
            }
			return s->getConstPointer( id );
		}

		// Otherwise is a constant
		return id;
    }
        
    // Return the left-hand side value
    virtual int getLHS( const State* s ) const{
		return getVarNum( s, _lhs );
	}
        
	// Return the right-hand side value     
	virtual int getRHS( const State* s ) const{
		return getVarNum( s, _rhs );
	}
	
	// Return
	virtual int getLeftVariable( const State *s ) const{
		if( !_lhs->accessMemory() )
			return _lhs->getID();
		return s->getRegister( _lhs->getID() );
	}
	
	virtual int getLeftVariableDomain( const State *s ) const{
		string vtype = _lhs->getVType();
		
		if( vtype == "register" )
			return MAX_VAL; // Max content of the memory
		return s->getNumRegisters(); // Size of the memory
	}

protected:
	string _name;
	Variable *_lhs; // if negative is a num
	Variable *_rhs; // if negative is a num
};

#endif
