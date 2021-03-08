#include "common.h"
#include "variable.h"
#include "state_descriptor.h"
#include "state.h"
#include "expression.h"
#include "condition.h"
#include "operation.h"
#include "action.h"
#include "domain.h"
#include "instance.h"
#include "parser.h"
#include "generalized_planning_problem.h"
#include "program_state.h"
#include "instruction.h"
#include "program.h"
#include "node.h"
#include "generalized_domain.h"
#include "heuristic.h"
#include "engine.h"


int main(  int argc, const char* argv[] ){
	if( argc < 3 ){
		cout << "[ERROR] Incorrect input." << endl;
		cout << "[INFO] Execute: ./main.bin <program_lines> <problem_folder/> <list of heuristic names>" << endl;
		cout << "[INFO] Example: ./main.bin 5 domains/heuristics/triangular-sum/ h6 h2 h5 h1 h3 h4" << endl;
		cout << "[INFO] List of heuristics is optional, the domain file must be domain.txt with all instances numbered from 1.txt to [n].txt" << endl;
		return -1;
	}

		
	int program_lines = stoi( argv[ 1 ] );	
	
	string problem_folder = string( argv[ 2 ] );
	
	ifstream ifs_domain( problem_folder + "domain.txt" );
	if( !ifs_domain ){
		cout << "[ERROR] domain.txt does not exist." << endl;
		return -2;
	}
	ifs_domain.close();
	
	Parser *parser = new Parser();
	Domain *dom = new Domain();
		
	if( !parser->parseDomain( dom, problem_folder + "domain.txt" ) ){
		cout << "[ERROR] while parsing domain.txt" << endl;
		delete dom;
		delete parser;
		return -3;
	}
	
	cout << "[INFO] Parsed domain" << endl;	
	
	// Generating the GP problem
	GeneralizedPlanningProblem *gpp = new GeneralizedPlanningProblem();
	gpp->addDomain( dom );
	
	int i = 1;	
	while( true ){
		Instance *ins = new Instance();
		string input_instance = problem_folder + to_string( i ) + ".txt";
		i++;
		
		ifstream ifs_instance( input_instance );
		if( !ifs_instance ){
			delete ins;
			break;
		}
		ifs_instance.close();
		
		if( parser->parseInstance( dom, ins, input_instance ) )
			gpp->addInstance( ins );
		else{
			delete ins;
			cout << "[ERROR] while parsing " << input_instance << endl;
			return -4;
		}
	}
	
	cout << "[INFO] Generalized Planning Problem created." << endl;
	
	GeneralizedDomain *gd = new GeneralizedDomain( dom, program_lines );
	
	cout << "[INFO] Generalized Domain created." << endl;
			
	Engine *engine = new BFS( program_lines, gd, gpp );
	
	cout << "[INFO] Engine created." << endl;
	
	string heuristics = "";
	// Default heuristics
	if( argc == 3 ){
		heuristics = "_h5_h1";
		engine->addHeuristic( new H5() );
		engine->addHeuristic( new H1() );
	}
	else{
		for( int i = 3; i < argc; i++ ){
			string h_name = string( argv[ i ] );
			if( h_name == "h1" ) engine->addHeuristic( new H1() );
			else if( h_name == "h2" ) engine->addHeuristic( new H2() );
			else if( h_name == "h3" ) engine->addHeuristic( new H3() );
			else if( h_name == "h4" ) engine->addHeuristic( new H4() ); 
			else if( h_name == "h5" ) engine->addHeuristic( new H5() ); 
			else if( h_name == "h6" ) engine->addHeuristic( new H6() );
            else if( h_name == "h7" ) engine->addHeuristic( new H7() );
            else if( h_name == "h8" ) engine->addHeuristic( new H8() );
            else if( h_name == "h9" ) engine->addHeuristic( new H9() );
            else{
				cout << "[ERROR] heuristic " << h_name << " is unknown." << endl;
				h_name = "";
			}
			heuristics += "_" + h_name;
		}
	}
	
	/*
	vector< int > ids = {2,2,0,7,0,13,7,0};
	Program *p = new Program( program_lines );
	for( int i = 0; i < program_lines; i++ )
		p->setInstruction( i, gd->getInstruction( i, ids[ i ] ) );
	cout << p->toString(false) << endl;
	vector< ProgramState* > vps = p->run( gpp );
	for( int i = 0; i < int( vps.size() ); i++ )
		cout << vps[ i ]->toString() << endl;
	*/
	
	cout << "[INFO] Searching..." << endl;
	
	Node *res = engine->solve();
	
	size_t problem_pos = problem_folder.find_first_of( "/" );
	string outfile = "experiments/" + heuristics;
	if( problem_pos != string::npos ){
		outfile = "experiments/" + problem_folder.substr( problem_pos + 1, problem_folder.length() - 2 - problem_pos ) + heuristics;
		
	}
		
	string output = "";
	
	if( res != NULL ){
		Program *p = res->getProgram();
		
		ofstream ofs_prog( outfile + ".prog" );
		ofs_prog << p->toString( false );
		ofs_prog.close();
		
		output += "[INFO] SOLUTION FOUND!!!\n";
		output += "[INFO] Expanded: " + to_string( engine->getExpanded() ) + "\n";
		output += "[INFO] Evaluated: " + to_string( engine->getEvaluated() ) + "\n";
		output += p->toString( false )  + "\n";
	
		cout << output;	
		cout << "[INFO] Program file: " << outfile << ".prog" << endl;	
	}	
	else{
		output += "[INFO] SOLUTION NOT FOUND :(\n";
		output += "[INFO] Expanded: " + to_string( engine->getExpanded() ) + "\n";
		output += "[INFO] Evaluated: " + to_string( engine->getEvaluated() ) + "\n";	
		cout << output;
	}

	ofstream ofs( outfile + ".out" );
	ofs << output;
	ofs.close();
	
	cout << "[INFO] Experiments file: " << outfile << ".out" << endl;	
	
	if( engine ) delete engine;
	if( parser ) delete parser;
}
