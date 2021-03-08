#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "common.h"
#include "state.h"
#include "condition.h"
#include "action.h"
#include "program_state.h"

class Instruction{
public:
	Instruction(){
	}
	
	virtual ~Instruction(){}
	
	// Maybe conds are not required, since only ZF and CF
	// are affected but already in a fix position
	virtual void addCond( Condition *c ){
		_conds.push_back( c );
	}
	
	virtual void setConds( vector< Condition* > &conds ){
		_conds = conds;
	}
	
	virtual bool isApplicable( ProgramState *ps ){
		return false;
	}
	
	virtual bool isGoalState( Instance *ins, ProgramState *ps ){
		return false;
	}
	
	virtual int apply( ProgramState *ps ){ return 0; }	
	
	virtual string toString( bool full_info = true ) const{ return "[INSTRUCTION] EMPTY\n"; }

protected:
	vector< Condition* > _conds;
};

class PlanningAction : public Instruction{
public:
	PlanningAction( Action *act ){
		_act = act;
	}
	
	virtual ~PlanningAction(){}
	
	virtual bool isApplicable( ProgramState *ps ){
		State *s = ps->getState();
		return _act->isApplicable( s );
	}
	
	virtual int apply( ProgramState *ps ){
		State *s = ps->getState();
		int line = ps->getLine();
		
		// State is updated in action function
		int res = _act->apply( s );
		
		ps->setLine( line + 1 );
		
		return res;
	}
	
	virtual string getName() const{
		return _act->getName();
	}
	
	virtual string getType() const{
		return _act->getType();
	}
	
	virtual string toString( bool full_info = true ) const{
		if( full_info )	
			return "[INSTRUCTION][PLANNING ACTION]:\n" + _act->toString();
		return _act->toString( full_info );
	}
	
private:
	Action *_act;
};

class Goto : public Instruction{
public:
	Goto( int dest = 0 ){
		_dest_line = dest;
	}
	
	~Goto(){}
	
	int getDestinationLine() const{
		return _dest_line;
	}
	
	virtual int apply( ProgramState *ps ){
		State *s = ps->getState();
		int line = ps->getLine();
		
		bool evals_true = true;
		for( int i = 0; evals_true && i < int( _conds.size() ); i++ ){
			evals_true = _conds[ i ]->evalCondition( s );
		}
		
		if( evals_true ) ps->setLine( line + 1 );
		else ps->setLine( _dest_line );
		
		return 0;
	}
	
	virtual string toString( bool full_info = true ) const{
		string ret = "";
		
		if( full_info ){
			ret = "[INSTRUCTION][GOTO]:\n"; 
			ret += "DESTINATION LINE: " + to_string( _dest_line ) + "\n";
		}
		else{
			ret = "goto(" + to_string( _dest_line ) + ",!(";
		}
		
		for( int i = 0; i < int( _conds.size() ); i++ )
			ret += _conds[ i ]->toString( full_info );
			
		if( !full_info ) ret += "))\n";
		return ret;
	}
	
private:
	int _dest_line;
};

class End : public Instruction{
public:
	End(){}
	
	~End(){}
	
	virtual bool isGoalState( Instance *ins, ProgramState *ps ){
		State *s = ps->getState();
		vector< Condition* > goal = ins->getGoalCondition();
		
		for( int i = 0; i < int( goal.size() ); i++ ){
			if( !goal[ i ]->evalCondition( s ) )
				return false;
		}
		
		return true;
	}
	
	virtual string toString( bool full_info = true ) const{
		string ret = "end\n";
		if( full_info ){
			ret= "[INSTRUCTION][END]:\n";
			for( int i = 0; i < int( _conds.size() ); i++ )
				if( _conds[ i ] )
					ret += _conds[ i ]->toString();
		}
		return ret;
	}
	
};

#endif
