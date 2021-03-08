#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "common.h"
#include "node.h"
#include "heuristic.h"
#include "generalized_planning_problem.h"
#include "generalized_domain.h"

struct CmpNodes{
    bool operator()(const Node* lhs, const Node* rhs) const{
        return lhs->f() > rhs->f();
    }
};

class Engine{
public:
	Engine( int program_lines, GeneralizedDomain *gd, GeneralizedPlanningProblem *gpp ){
		_evaluated_nodes = 0;
		_expanded_nodes = 0;
		_program_lines = program_lines;
		_gd = gd;
		_gpp = gpp;
	}
	
	virtual ~Engine(){
		for( int i = 0; i < int( _heuristics.size() ); i++ ){
			if( _heuristics[ i ] )
				delete _heuristics[ i ];
		}
		if( _gd ) delete _gd;
		if( _gpp ) delete _gpp;
	}
	
	virtual bool isEmpty() const = 0;
	
	// Add a node to searching data structure
	virtual void addNode( Node *node ) = 0;
	
	// Select the next Node and update the data structure
	virtual Node* selectNode() = 0;
	
	// Check if goal conditions hold in the current Node
	virtual bool isGoal( Node *node ) = 0;
	
	// Expand the current Node, adding all non-visited childs
	virtual vector< Node* > expandNode( Node *node ) = 0;
	
	// accumulated cost
	virtual int g( Node *node ) = 0;
	
	// heuristic functions
	virtual vector<int> h( Node *node ) = 0;
	
	// evaluation functions
	virtual vector<int> f( Node *node ) = 0;
	
	// Solve the problem starting from init Node
	virtual Node* solve() = 0;
	
	// Add a new heuristic to the engine
	void addHeuristic( Heuristic *new_h ){ 
		cout << "[INFO] Heuristic " << new_h->getName() << " added." <<  endl;
		_heuristics.push_back( new_h );
	}
	
	int getExpanded(){
		return _expanded_nodes;
	}
	
	int getEvaluated(){
		return _evaluated_nodes;
	}
	
	
protected:
	int _evaluated_nodes;
	int _expanded_nodes;
	int _program_lines;
	vector< Heuristic* > _heuristics;
	GeneralizedPlanningProblem *_gpp;
	GeneralizedDomain *_gd;
};

class BFS : public Engine{
public:
	BFS(int program_lines, GeneralizedDomain *gd, GeneralizedPlanningProblem *gpp) : Engine( program_lines, gd, gpp ){
	}
	
	virtual ~BFS(){ 
		//for( int i = 0; i < int( _closed.size() ); i++ )
		//	if( _closed[ i ] ) delete _closed[ i ];
		while( !isEmpty() ){
			Node *n = _open.top();
			_open.pop();
			if( n ) delete n;
		}
	}
	
	virtual bool isEmpty() const{
		return _open.empty();
	}
	
	virtual void addNode( Node *node ){
		_open.push( node );
	}
	
	virtual Node* selectNode(){
		return _open.top();
	}
	
	void freeProgramStates( vector< ProgramState* > &vps ){
		for( int i = 0; i < int( vps.size() ); i++ ){
			if( vps[ i ] )
				delete vps[ i ];
		}
	}
	
	virtual bool isGoal( Node *node ){
		//Program *p = node->generateProgram( _program_lines );
		Program *p = node->getProgram();
		vector< ProgramState* > vps = p->run( _gpp );
		
		// Some error ocurred during execution
		if( vps.size() == 0 )
			return false;
		
		for( int i = 0; i < int( vps.size() ); i++ ){
			int line = vps[i]->getLine();
			Instruction *ins = p->getInstruction( line );
			if( ins == NULL ){
				freeProgramStates( vps );
				return false;
			}
			
			End *end = dynamic_cast<End*>( ins );
			if( end ){
				if( !end->isGoalState( _gpp->getInstance( i ), vps[i] ) ){
					freeProgramStates( vps );
					return false;
				}
			}
			else{
				freeProgramStates( vps );
				return false;
			}
		}
		
		freeProgramStates( vps );
		
		return true;
	}
	
	virtual vector< Node* > expandNode( Node *node ){
		int pc_max = -1; 
		//Program *p = node->generateProgram( _program_lines );
		Program *p = node->getProgram(); 
		
		// find the next line to program
		vector< ProgramState* > vps = p->run( _gpp );
		for( int i = 0; i < int( vps.size() ); i++ ){
			int line = vps[ i ]->getLine();
			if( p->getInstruction( line ) == NULL ){
				pc_max = max( pc_max, line );
			}
		} 
		
		freeProgramStates( vps );
		
		// Failure case when next valid line not found
		if( pc_max == -1 or p->getInstruction( pc_max ) != NULL ){
			//delete p;
			return vector<Node*>();
		}
			
		vector< Instruction* > vi = _gd->getInstructions( pc_max );
		
		// [1] Symmetry Breaking - only Gotos after CMP
		bool only_gotos = false;
		bool gotos_enabled = false;
		if( pc_max > 0 ){
			PlanningAction *pa = dynamic_cast<PlanningAction*>( p->getInstruction( pc_max-1 ) );
			only_gotos = ( pa &&  ( (pa->getName()).substr(0,3) == "cmp" ) );
			gotos_enabled = ( pa && (pa->getType() == "math") );
		}
		
		vector< Node* > childs;
		int maxi = max( 1, int(_heuristics.size() ) );
		for( int i = 0; i < int( vi.size() ); i++ ){
			// [1] S.B.
			if( only_gotos ){
				Goto *g = dynamic_cast<Goto*>( vi[ i ] );
				if( !g ) continue;
			}
			if( !gotos_enabled ){
                Goto *g = dynamic_cast<Goto*>( vi[ i ] );
                if( g ) continue;
			}
			
			//Node *n2 = new Node( vi[ i ], pc_max, INF, vector<int>(maxi,0), node );
			//Program *p2 = n2->generateProgram( _program_lines );
			Program *p2 = p->copy();
			p2->setInstruction( pc_max, vi[ i ] );
			//Node *n2 = new Node( p2, vector<int>(maxi,0) );
			//cout << n2->toString() << endl;
			vector< ProgramState* > vps2 = p2->run( _gpp );
			
			if( vps2.size() != 0 ){
				childs.push_back( new Node( p2, vector<int>(maxi,0) ) );
			}
			else if( p2 ){
				delete p2;
			}
			//if( p2 ) delete p2;
			freeProgramStates( vps2 );
		}
		
		return childs;
	}
	
	virtual int g( Node *node ){
		return 0;
		//return node->g();
	}
	
	virtual vector<int> h( Node *node ){
		//Program *p = node->generateProgram( _program_lines );
		Program *p = node->getProgram();
		vector< int > val_h = vector<int>( _heuristics.size(), 0 );
		for( int i = 0; i < int( _heuristics.size() ); i++ ){
			val_h[ i ] = _heuristics[ i ]->compute( p, _gpp );
		}
		//if( p ) delete p;
		return val_h;
	}
	
	virtual vector<int> f( Node *node ){
		return h( node );
		/*
		int val_g = g( node );
		vector< int > val_f = h( node );
		for( int i = 0; i < int( val_f.size() ); i++ ){
			val_f[ i ] += val_g;
		}
		if( val_f.size() == size_t(0) )
			return vector<int>(1,val_g);
		return val_f;
		*/
	}
	
	int distance( const Node *a, const Node *b ){
		//return 1; // default value
		return 0;
	}
	
	virtual Node* solve(){		
		Node *root = new Node( new Program( _program_lines ), vector<int>( _heuristics.size(), 0 ) );								
		
		addNode( root );
		
		while( !isEmpty() ){		
			_expanded_nodes++;
			
			Node *current = selectNode();
			
			// remove current node from open
			_open.pop(); 
			
			if( _expanded_nodes%10000 == 0 ){			
				cout << "[ENGINE] EXPANDED: " << _expanded_nodes << "; EVALUATED: " << _evaluated_nodes << "; OPEN QUEUE SIZE: " << _open.size() << endl;
				//Program *p = current->generateProgram( _program_lines );
				Program *p = current->getProgram();
				cout << p->toString( false ) << endl;
				cout << current->toString() << endl;
				//if( p ) delete p;
			}
			
			//if( isGoal( current ) )
			//	return current;
			
			//int g = current->g();
			vector< Node* > childs = expandNode( current );

			delete current;
			
			for( int i = 0; i < int( childs.size() ); i++ ){ 
				if( childs[ i ] == NULL ) continue;
				//int next_g = g + distance( current, childs[ i ] );
				
				//if( next_g < childs[ i ]->g() ){
				_evaluated_nodes++;
				//childs[ i ]->setG( next_g );
				childs[ i ]->setF( h( childs[ i ] ) );
				
				if( isGoal( childs[ i ] ) ){
					addNode( childs[ i ] );
					for( int j = i+1; j < int( childs.size() ); j++ )
						if( childs[ j ] )
							delete childs[ j ];
					return childs[ i ];
				}
				
				// Standard A* but no chance to repeat programs
				// so nodes are always pushed into priority queue
				//if( _open.find( childs[i] ) == _open.end() )
				addNode( childs[ i ] );
				//}
			}
		}
		
		return NULL;
	}
	
private:
	priority_queue< Node*, vector< Node* >, CmpNodes > _open;
	//vector< Node* > _closed;
};

#endif
