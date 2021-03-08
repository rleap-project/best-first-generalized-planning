#ifndef __GENERALIZED_DOMAIN_H__
#define __GENERALIZED_DOMAIN_H__

#include "common.h"
#include "state_descriptor.h"
#include "action.h"
#include "domain.h"
#include "condition.h"
#include "instruction.h"

class GeneralizedDomain{
public:
	// Instructions can be created only once but we keep the logic
	// of which actions can be programmed in each line here, so the
	// engine does not have to reason about it
	GeneralizedDomain( Domain *d, int program_lines ){
		StateDescriptor *sd = d->getStateDescriptor();
	
		_instructions_line.resize( program_lines );
		
		// Planning Actions
		vector< Action* > actions = d->getActions();
		// [2] Symmetry Breaking - Only End can be programmed in last line
		for( int i = 0; i + 1 < program_lines; i++ ){
			for( int j = 0; j < int( actions.size() ); j++ ){
				_instructions_line[ i ].push_back( new PlanningAction( actions[ j ] ) );
			}
		}
		
		// Extending planning actions with counter and pointer instructions
		vector< string > prog_vars = sd->getVars();
		// [2] Symmetry Breaking - extra actions cannot be programmed in the last two lines
		for( int line = 0; line+2<program_lines;line++ ){
			for( int i = 0; i < int( prog_vars.size() ); i++ ){
				// zf and cf flags are indirectly modified by other pointer math updates
				if( prog_vars[ i ] == "zf" or prog_vars[ i ] == "cf" )
					continue;
				string vt = sd->getVType( prog_vars[ i ] );
				if( vt == "pointer" or vt == "counter" ){
					
					// Increment counter or pointer
					Action *inc_act = new Action("inc("+prog_vars[i]+")");
					
					Condition *inc_cond = new Add(
					        new Variable( prog_vars[i], vt, sd->getID( prog_vars[i] ) ),
					        new Variable( "1", "constant", 1 ), sd->getNumRegisters() );
					inc_act->addCondition( inc_cond );
					
					Operation *add_assign_op = new AddAssign(
					        new Variable( prog_vars[i], vt, sd->getID( prog_vars[i] ) ),
					        new Variable( "1", "constant", 1 ) );
					inc_act->addOperation( add_assign_op );
					
					_extra_actions.push_back( inc_act );
					
					Instruction *inc_ins = new PlanningAction( inc_act );					
					_instructions_line[ line ].push_back( inc_ins );
					
					// Decrement counter or pointer
					Action *dec_act = new Action("dec("+prog_vars[i]+")");
					
					Condition *dec_cond = new Subtract(
					        new Variable( prog_vars[i], vt, sd->getID( prog_vars[i] ) ),
					        new Variable( "1", "constant", 1 ) );
					dec_act->addCondition( dec_cond );
					
					Operation * sub_assign_op = new SubtractAssign(
					        new Variable( prog_vars[i], vt, sd->getID( prog_vars[i] ) ),
					        new Variable( "1", "constant", 1 ) );
					dec_act->addOperation( sub_assign_op );
					
					_extra_actions.push_back( dec_act );
					
					Instruction *dec_ins = new PlanningAction( dec_act );
					_instructions_line[ line ].push_back( dec_ins );
				}
				
				if( vt == "pointer" or vt == "const-pointer" ){
					for( int j = 0; j < int( prog_vars.size() ); j++ ){
						if( prog_vars[ j ] == "zf" or prog_vars[ j ] == "cf" or i == j )
							continue;
						string vt2 = sd->getVType( prog_vars[ j ] );
						
						if( vt2 != "pointer" and vt2 != "const-pointer" )
							continue;
						if( vt == "const-pointer" and vt2 == "const-pointer" )
							continue;
							
						// [3] Symmetry breaking - allow to compare only in one direction
						if( j < i ){	
							// CMP instruction between two pointers
							Action *cmp_act = new Action("cmp("+prog_vars[i]+","+prog_vars[j]+")");
							// NO CONDS
							Operation *cmp_op = new Compare(
							        new Variable( prog_vars[i], vt, sd->getID( prog_vars[i] ) ),
							        new Variable( prog_vars[j], vt2, sd->getID( prog_vars[j] ) ) );
							cmp_act->addOperation( cmp_op );
							
							_extra_actions.push_back( cmp_act );
						
							Instruction *cmp_ins = new PlanningAction( cmp_act );
							_instructions_line[ line ].push_back( cmp_ins );

                            // [3] Symmetry breaking - allow to compare only in one direction
                            // CMP instruction between two memory registers given two pointers
                            Action *cmp_mem_act = new Action( "cmp(*"+prog_vars[i]+",*"+prog_vars[j]+")");
                            //NO CONDS
                            Operation *cmp_mem_op = new Compare(
                                    new Variable( prog_vars[i], vt, sd->getID( prog_vars[i] ), true ),
                                    new Variable( prog_vars[j], vt2, sd->getID( prog_vars[j] ), true ) );
                            cmp_mem_act->addOperation( cmp_mem_op );

                            _extra_actions.push_back( cmp_mem_act );

                            Instruction *cmp_mem_ins = new PlanningAction( cmp_mem_act );
                            _instructions_line[ line ].push_back( cmp_mem_ins );
						}

						//if( vt == "pointer" and vt2 == "pointer" ) {
                        if( vt == "pointer" ){
                            // SET a pointer to another one
                            Action *set_act = new Action("set(" + prog_vars[i] + "," + prog_vars[j] + ")");
                            //NO CONDS
                            Operation *assign_op = new Assign(new Variable(prog_vars[i], vt, sd->getID(prog_vars[i])),
                                                              new Variable(prog_vars[j], vt2, sd->getID(prog_vars[j])));
                            set_act->addOperation(assign_op);

                            _extra_actions.push_back(set_act);

                            Instruction *set_ins = new PlanningAction(set_act);
                            _instructions_line[line].push_back(set_ins);
                        }
					}
				}
			}
		}	
		
		// Conditional Gotos
		// Set 4 different conditions ZF, !ZF, CF, !CF
		_conds.resize( 4 );
		_conds[ 0 ] = new Equals(
		        new Variable( "zf", "pointer", sd->getID( "zf" ) ),
		        new Variable( "1", "constant", 1 ) );
		_conds[ 1 ] = new Equals(
		        new Variable( "zf", "pointer", sd->getID( "zf" ) ),
		        new Variable( "0", "constant", 0 ) );
		_conds[ 2 ] = new Equals(
		        new Variable( "cf", "pointer", sd->getID( "cf" ) ),
		        new Variable( "1", "constant", 1 ) );
		_conds[ 3 ] = new Equals(
		        new Variable( "cf", "pointer", sd->getID( "cf" ) ),
		        new Variable( "0", "constant", 0 ) );
			
		// Only End can be programmed in last line
		for( int from = 0; from + 1 < program_lines; from++ ){
			for( int to = 0; to < program_lines; to++ ){
				// A goto cannot jump over itself or next line
				if( from == to || from+1 == to ) 
					continue;				
				for( int k1 = 0; k1 < 2; k1++ ){
					for( int k2 = 2; k2 < 4; k2++ ){
						Instruction *ins = new Goto( to );
						ins->addCond( _conds[ k1 ] );
						ins->addCond( _conds[ k2 ] );
						_instructions_line[ from ].push_back( ins );
					}
				}
			}
		}
		
		// End
		for( int i = 0; i < program_lines; i++ ){
			_instructions_line[ i ].push_back( new End() );
		}

        //cout << toString() << endl;
	}
	
	~GeneralizedDomain(){
		for( int i = 0; i < int( _instructions_line.size() ); i++ ){
			for( int j = 0; j < int( _instructions_line[ i ].size() ); j++ ){
				//cout << "LINE #" << i << " id: " << j << endl;
				//cout << _instructions_line[ i ][ j ]->toString() << endl;
				if( _instructions_line[ i ][ j ] ) 
					delete _instructions_line[ i ][ j ];
			}
		}
		
		for( int i = 0; i < int( _conds.size() ); i++ ){
			if( _conds[ i ] )
				delete _conds[ i ];
		}
		
		for( int i = 0; i < int( _extra_actions.size() ); i++ ){
			if( _extra_actions[ i ] )
				delete _extra_actions[ i ];
		}
	}
	
	Instruction* searchInstruction( int program_line, const string &name ){
		if( program_line >= int( _instructions_line.size() ) )
			return NULL;
		for( int i = 0; i < int( _instructions_line[ program_line ].size() ); i++ ){
			//cout << (_instructions_line[ program_line ][ i ]->toString( false )).size() << " " << _instructions_line[ program_line ][ i ]->toString( false ) << endl;
			//cout << name.size() << " " << name << endl << endl;		
			if( _instructions_line[ program_line ][ i ]->toString( false ) ==  (name+"\n") )
				return _instructions_line[ program_line ][ i ];
		}
		return NULL;
	}
	
	vector< vector< Instruction* > > getInstructions(){
		return _instructions_line;
	}
	
	vector< Instruction* > getInstructions( int program_line ){
		if( program_line >= int( _instructions_line.size() ) )
			return vector<Instruction*>();
		return _instructions_line[ program_line ];
	}
	
	Instruction* getInstruction( int program_line, int id ){
		return _instructions_line[ program_line ][ id ];
	}
	
	string toString(){
		string ret = "[GENERALIZED DOMAIN]:\n";
		for( int i = 0; i < int( _instructions_line.size() ); i++ ){
			ret += "[LINE " + to_string( i ) +"]:\n";
			for( int j = 0; j < int( _instructions_line[ i ].size() ); j++ ){
				//ret += "ID: " + to_string(j) + " ";
				ret += _instructions_line[ i ][ j ]->toString();
			}
		}
		return ret;
	}
	
	
private:
	vector< vector< Instruction* > > _instructions_line;
	vector< Condition* > _conds;
	vector< Action* > _extra_actions;
};

#endif
