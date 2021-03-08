#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "common.h"
#include "program_state.h"
#include "instruction.h"
#include "generalized_planning_problem.h"

class Program{
public:
	Program( int program_lines = 1 ){
		_instructions = vector<Instruction*>( program_lines, NULL );
        _num_of_steps = 0;
	}
	
	Program( Program* p ){
		vector< Instruction* > vi = p->getInstructions();
		_instructions.resize( vi.size() );
		for( int i = 0; i < int( _instructions.size() ); i++ )
			_instructions[ i ] = vi[ i ];
        _num_of_steps = 0;
	}
	
	~Program(){
	}
	
	Program* copy(){
		return new Program( this );
	}
	
	vector< Instruction* > getInstructions(){
		return _instructions;
	}
	
	bool setInstruction( int pl, Instruction* ins ){
		if( pl >= int( _instructions.size() ) )
			return false;
		_instructions[ pl ] = ins;
		return true;
	}
	
	Instruction* getInstruction( int pl ){
		if( pl >= int( _instructions.size() ) )
			return NULL;
		return _instructions[ pl ];
	}
	
	int getNumInstructions() const{
		return int( _instructions.size() );
	}
	
	bool haltingCondition( ProgramState* ps, int &error ){
		int line = ps->getLine();
		//cout << line << " " << ps->toString() << endl;
		// EMPTY line is a halting condition (no transition defined)
/*		PlanningAction* ins_act = dynamic_cast< PlanningAction* > ( _instructions[ line ] );
		Goto* ins_goto = dynamic_cast< Goto* > ( _instructions[ line ] );
		End* ins_end = dynamic_cast< End* > ( _instructions[ line ] );
		if( ins_act == NULL and ins_goto == NULL and ins_end == NULL ) return true;*/
		if( _instructions[ line ] == NULL ){
			return true;
		}
		//cout << "INSTRUCTION" << endl;
		//cout << _instructions[ line ]->toString() << endl;
		
		// Either True or False evaluation of End instructions is a halting condition
		End* ins_end = dynamic_cast< End* > ( _instructions[ line ] );
		if( ins_end ) return true; 
		
		// If its a planning action, it must be applicable 
		PlanningAction* ins_act = dynamic_cast< PlanningAction* > ( _instructions[ line ] );
		if( ins_act && !ins_act->isApplicable( ps ) ){
            #ifdef VALIDATOR
		    cout << ps->toString() << endl;
		    cout << ins_act->toString() << endl;
            #endif
			error = -2; // ERROR 2: inapplicable program
			return true;
		}
		
		return false;
	}
	
	bool checkGoal( ProgramState *ps, Instance *ins, int &error ){
		int line = ps->getLine();
		if( _instructions[ line ] == NULL ) return false;
		End *end = dynamic_cast<End*>( _instructions[ line ] );
		if( end ){
			if( !end->isGoalState( ins, ps ) ){
                #ifdef VALIDATOR
			    cout << ps->toString() << endl;
                #endif
				error = -1; // ERROR 1: Incorrect program
				return false;
			}
		}
		else return false;
		return true;
	}
	
	vector< ProgramState* > run( GeneralizedPlanningProblem *gpp, bool infinite_detection = true ){
        _num_of_steps = 0;

		int num_instances = gpp->getNumInstances();
		StateDescriptor *sd = (gpp->getDomain())->getStateDescriptor();
		
		// One program state per instance
		vector< ProgramState* > pss( num_instances ) ;
		for( int i = 0; i < num_instances; i++ )
			pss[ i ] = new ProgramState();
		
		for( int id = 0; id < num_instances; id++ ){ 
			// Initialize local initial state
			Instance *ins = gpp->getInstance( id );			
			ProgramState *ps = pss[ id ];
						
			State *s = ( ins->getInitialState() )->copy();
			int line = ps->getLine();
			ps->setState( s );
			
			// For detecting infinite loops
			// (it can be enhanced with a hash or bigint identifier)
			set< vector<int> > visited;
			vector< vector< int > > vars = s->getVars();
			
			vector< int > state_id( 1, ps->getLine() );
			
			if( infinite_detection ){
				for( int i = 0; i < int( vars.size() ); i++ ){
					for( int j = 0; j < int( vars[i].size() ); j++ ){
						state_id.push_back( vars[i][j] );
					}
				}
			}
			
			int error = 0;
			while( !haltingCondition( ps, error ) ){
				if( infinite_detection ){
					// Checking infinite loop
					Goto *g = dynamic_cast<Goto*>( _instructions[ line ] );
					if( g ){
						vars = s->getVars();
						state_id[ 0 ] = ps->getLine();
						for( int i = 0, k = 1; i < int( vars.size() ); i++ )
							for( int j = 0; j < int( vars[i].size() ); j++, k++ )
								state_id[ k ] = vars[i][j];
						// Infinite loop detected
						if( visited.find( state_id ) != visited.end() ){
							error = -3; // ERROR 3: Infinite program
							break;
						}
						
						visited.insert( state_id ); 
					}
				}
				
				//cout << "APPLYING INSTRUCTION:" << endl;
				//cout << _instructions[ line]->toString() << endl;
				//cout << s->toString() << endl<< endl;
				
				// Retrieving program line
				line = ps->getLine();

				// Applying current instruction						
				int res = _instructions[ line ]->apply( ps );
				_num_of_steps++;

				// Mathematical planning actions update zero and carry flag
				PlanningAction *act = dynamic_cast< PlanningAction* >( _instructions[ line ] );				
				if( act && act->getType() == "math" ){
					s->setPointer( sd->getID("zf"), ( res == 0 ?1:0) ); // zero-flag
					s->setPointer( sd->getID("cf"), ( res > 0 ? 1:0) ); // carry-flag
				}
                //cout << line << " " << res << " " <<  s->toString(sd) << endl<< endl;
			}
			
			checkGoal( ps, ins, error );
			
			// [ENHANCEMENT] Continue execution in validation for coverage metric
			#ifdef VALIDATOR
			if( error < 0 )
				cout << "INSTANCE #" << (id+1) << " ERROR " << error << "... :(" << endl;
			else
				cout << "INSTANCE #" << (id+1) << " SOLVED!" << endl;
			#endif
			
			if( error < 0 ){		
				//cout << toString( false ) << endl;
				//cout << "[ERROR] with id " << error << " while running program." << endl;
				for( int i = 0; i < int( pss.size() ); i++ ){
					//cout << "I: " << i << " " << pss[ i ]->toString() << endl;
					if( pss[i] ) delete pss[i];
				}
				return vector<ProgramState*>();
			}
		}
		
		return pss;
	}

	int getNumOfSteps() const{
	    return _num_of_steps;
	}
	
	string toString( bool full_info = true ) const{
		string ret = "";
		if( full_info ){
			ret = "[PROGRAM]:\n";
			for( int i = 0; i < int( _instructions.size() ); i++ ){
				ret += "LINE " + to_string( i ) + ": ";
				if( _instructions[ i ] != NULL ) 
					ret += _instructions[ i ]->toString();
				else ret += "[INSTRUCTION] EMPTY\n";
			}
		}
		else{
			for( int i = 0; i < int( _instructions.size() ); i++ ){
				ret += to_string( i ) + ". ";
				if( _instructions[ i ] != NULL )
					ret += _instructions[ i ]->toString( full_info );
				else ret += "empty\n";
			}
		}
		return ret;
	}
	
private:
	vector< Instruction* > _instructions;
	int _num_of_steps;
};

#endif
