#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "common.h"
#include "instruction.h"
#include "program.h"
#include "program_state.h"
#include "generalized_planning_problem.h"

class Heuristic{
public:
	Heuristic( const string &name = "heuristic" ) : _name( name ){
	}
	
	virtual ~Heuristic(){
	}
	
	virtual string getName() const{
		return _name;
	}
	
	// Program-based or Performance-based heuristic
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL  ){
		return 0;
	}
private:
	string _name;
};

// OLD H1
class H7 : public Heuristic{
public:
	H7() : Heuristic( "h7" ){}
	
	virtual ~H7(){}
	
	// Compute nested gotos (there could be multiple strategies)
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
		int res = 0;
		vector< Instruction* > instructions = p->getInstructions();
		vector< int > affected_lines( instructions.size(), 0 );
		for( int line = 0; line < int( instructions.size() ); line++ ){
			Goto *g = dynamic_cast<Goto*>( instructions[ line ] );
			if( g ){
				int aux_line = g->getDestinationLine();
				int dir = (aux_line > line ? -1:1);
				while( aux_line != line ){
					affected_lines[ aux_line ]++;
					aux_line += dir;
				}
				affected_lines[ line ]++;
			}
		}
		res = *max_element(affected_lines.begin(), affected_lines.end());
		return res;
	}
};

// OLD H2
class H1: public Heuristic{
public:
	H1() : Heuristic( "h1" ){}
	
	virtual ~H1(){}
	
	// Compute number of gotos of a program
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
		int res = 0;
		vector< Instruction* > instructions = p->getInstructions();
		for( int i = 0; i < int( instructions.size() ); i++ ){
			Goto *g = dynamic_cast<Goto*>( instructions[ i ] );
			if( g ) res++;
		}
		return res;		
	}
};

// OLD H3
class H2 : public Heuristic{
public:
	H2() : Heuristic( "h2" ){}
	
	virtual ~H2(){}
	
	// Compute the number of empty instructions
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
		int res = 0;
		vector< Instruction* > instructions = p->getInstructions();
		for( int i = 0; i < int( instructions.size() ); i++ ){
			if( instructions[ i ] == NULL )
				res++;
		}
		return res;		
	}
};

class H4 : public Heuristic{
public:
	H4() : Heuristic( "h4" ){}
	
	virtual ~H4(){}
	
	// Run the program over instances and return the last empty line found
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL){
		int res = p->getNumInstructions(), pc_max = -1;
		vector<ProgramState*> vps = p->run( gpp );
		for( int i = 0; i < int( vps.size() ); i++ ){
			int line = vps[ i ]->getLine();
			if( p->getInstruction( line ) == NULL )
				pc_max = max( pc_max, line );
			
			delete vps[i];
		}
		if( pc_max == -1 ) return res;
		return res - pc_max;
	}
};

class H5 : public Heuristic{
public:
	H5() : Heuristic( "h5" ){}
	
	virtual ~H5(){}
	
	// Computing squared distance to goal after each execution.
	// This assumes state variables are in the same domain and goals as equalities
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL){
		long long res = 0;
		vector<ProgramState*> vps = p->run( gpp );
		//long long num_goal_variables = 0;
		//long long total_variable_domains = 0;
		for( int i = 0; i < int( vps.size() ); i++ ){
			State *s = vps[ i ]->getState();
			Instance *ins = gpp->getInstance( i );
			vector< Condition* > goal = ins->getGoalCondition();
			long long ins_res = 0;
			for( int j = 0; j < int( goal.size() ); j++ ){
				long long lhs = goal[j]->getLHS( s );
				long long rhs = goal[j]->getRHS( s );
				// Normalize by the domain of state variable and accumulate
                //long long lhs_domain = max(1, goal[j]->getLeftVariableDomain( s ) );
				//ins_res += (((lhs-rhs)*(lhs-rhs))/lhs_domain); // this assumes that goal are equalities
				ins_res += ((lhs-rhs)*(lhs-rhs));
				//total_variable_domains += lhs_domain;
			}
			// normalize by the number of goal variables
			//ins_res /= (long long)( goal.size() );
			// accumulate the instance
			//res += (long long)(sqrt(ins_res ));
			res += ins_res;
			//num_goal_variables += (long long)goal.size();
			delete vps[i];
		}
		//if( res < 10 )
		//	cout << res << " " << vps.size() << endl;
		return int(res);
		//return (int)(res/num_goal_variables);		
		//return (int)(res/int(vps.size()));
	}
};

// OLD H6
class H3 : public Heuristic{
public:
	H3() : Heuristic( "h3" ){}
	
	virtual ~H3(){}
	
	bool isValidInstruction( Instruction *ins ){
		PlanningAction *pa = dynamic_cast<PlanningAction*>( ins );
		Goto *g = dynamic_cast<Goto*>( ins );
		End *end = dynamic_cast<End*>( ins );
		return ((pa != NULL) or (g != NULL) or (end != NULL));
	}
	
	virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
		int res = 0;
		vector< Instruction* > instructions = p->getInstructions();
		vector< bool > affected_lines( instructions.size(), false );
		for( int line = 0; line < int( instructions.size() ); line++ ){
			if( affected_lines[ line ] ){ 
				res++;
				continue;
			}
//			PlanningAction *pa = dynamic_cast<PlanningAction*>( instructions[line] );
			if( !isValidInstruction( instructions[ line ] ) ) continue;
			//if( pa ){
				for( int next_lines = line+1; next_lines < int( instructions.size() ); next_lines++ ){
					if( affected_lines[ next_lines ] ) continue;
					if( !isValidInstruction( instructions[ next_lines ] ) ) continue;
					//PlanningAction *pa2 = dynamic_cast<PlanningAction*>( instructions[ next_lines ] );
					//if( pa2 && pa->toString(false)==pa2->toString(false)){
					if( instructions[ line ]->toString( false ) == instructions[ next_lines ]->toString( false ) ){
						affected_lines[ next_lines ] = true;
					}	
				}
			//}
		}
		
		return res;
	}
};

// OLD H7
class H6 : public Heuristic{
public:
    H6() : Heuristic( "h6" ){}

    virtual ~H6(){}

    // Computing number of execution steps.
    // This assumes state variables are in the same domain and goals as equalities
    virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL){
        vector<ProgramState*> vps = p->run( gpp );
        int num_of_steps = p->getNumOfSteps();

        for( int i = 0; i < int( vps.size() ); i++ ) {
            delete vps[i];
        }

        return num_of_steps;
    }
};

class H8 : public Heuristic{
public:
    H8() : Heuristic( "h8" ){}

    virtual ~H8(){}

    // Compute number of math instructions (priorize planning actions)
    virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
        int res = 0;
        vector< Instruction* > instructions = p->getInstructions();
        for( int line = 0; line < int( instructions.size() ); line++ ) {
            PlanningAction *pa = dynamic_cast<PlanningAction *>( instructions[line] );
            if( pa && pa->getType() != "memory" )
            //if( pa && pa->getType() == "math" )
                res++;
        }
        return res;
    }
};

class H9 : public Heuristic{
public:
    H9() : Heuristic( "h9" ){}

    virtual ~H9(){}

    virtual int compute( Program *p, GeneralizedPlanningProblem *gpp = NULL ){
        vector<ProgramState*> vps = p->run( gpp );
        int res = p->getNumOfSteps(); // G function

        for( int i = 0; i < int( vps.size() ); i++ ){
            State *s = vps[ i ]->getState();
            Instance *ins = gpp->getInstance( i );
            vector< Condition* > goal = ins->getGoalCondition();
            for( int j = 0; j < int( goal.size() ); j++ ){
                int lhs = goal[j]->getLHS( s );
                int rhs = goal[j]->getRHS( s );
                res += ((lhs-rhs)*(lhs-rhs)); // H function
            }
            delete vps[i];
        }
        return res;
    }
};

#endif
