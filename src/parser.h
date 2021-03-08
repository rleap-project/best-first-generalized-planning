#ifndef __PARSER_H__
#define __PARSER_H__

#include "common.h"
#include "variable.h"
#include "state_descriptor.h"
#include "state.h"
#include "condition.h"
#include "operation.h"
#include "action.h"
#include "domain.h"
#include "instance.h"

class Parser{
public:
	Parser(){
	}
	
	~Parser(){
		
	}

	string errorMessage( const string &expected_word = "", const string &readed_word = "" ) const{
		return "Expected ''" + expected_word + "'' but ''" + readed_word + "'' readed.";
	}

	bool isDigit( const string &s ) const{
		return (s.find_first_not_of("0123456789") == string::npos );
	}

	Variable* readVar( string &name, StateDescriptor *sd){
		// Remove the access to memory from the name
		bool var_access_memory = ( name[ 0 ] == '*' );
		if( var_access_memory )
			name = name.substr( 1 );

		bool var_constant = isDigit( name );
		string vtype = "";

		// If the variable is a digit then it is a constant
		if( var_constant ) vtype = "constant";
		else vtype = sd->getVType( name );

		// No variable type detected
		if( "" == vtype ) 
			return NULL;
		
		// Only pointers and constant-pointers have access to memory
		if( "pointer" != vtype and "const-pointer" != vtype and var_access_memory )
			return NULL;

		int id;
		if( var_constant ) id = atoi( name.c_str() );
		else id = sd->getID( name );

		// State variable does not exist
		if( -1 == id )
			return NULL;

		return (new Variable( name, vtype, id, var_access_memory ));
	}

	Condition* readCondition( const string &name, Variable *lhs, Variable *rhs, int max_val = MAX_VAL ){
		if( ">=" == name ) return new GreaterEqual( lhs, rhs ); 
		else if( "<=" == name ) return new LesserEqual( lhs, rhs );
		else if( ">" == name ) return new Greater( lhs, rhs );
		else if( "<" == name ) return new Lesser( lhs, rhs );
		else if( "=" == name ) return new Equals( lhs, rhs );
		else if( "+" == name ) return new Add( lhs, rhs, max_val );
		else if( "-" == name ) return new Subtract( lhs, rhs );
		else if( "!=" == name ) return new Different( lhs, rhs );
		
		return NULL;
	}

	Operation* readOperation( const string &name, Variable *lhs, Variable *rhs ){
		if( "+=" == name ) return new AddAssign( lhs, rhs );
		else if( "-=" == name ) return new SubtractAssign( lhs, rhs );
		else if( "=" == name ) return new Assign( lhs, rhs );
		else if( "-" == name ) return new Compare( lhs, rhs );
		
		return NULL;
	}
	
	// In charge of parsing and creating a domain
	bool parseDomain( Domain *dom, const string &file_name = "" ){
		ifstream ifs( file_name.c_str() );
		
		string name;
		string var;
		string vtype, vtype2, atype;
		string word;

		// DOMAIN NAME
		ifs >> name;			
		
		//Domain *dom = new Domain( name );
		dom->setName( name );
		
		// STATE DESCRIPTOR
		if( ifs.eof() ){
			cerr << errorMessage( "STATE", "END OF FILE" ) << endl;
			return false;
		}
		
		ifs >> word;
		
		if( word != "STATE" ){
			cerr << errorMessage( "STATE", word ) << endl;
			return false;
		}
		
		if( ifs.eof() ) {
			cerr << errorMessage( "DESCRIPTOR:", "END OF FILE" ) << endl;
			return false;
		}
		
		ifs >> word;
		
		if( word != "DESCRIPTOR:" ){
			cerr << errorMessage( "DESCRIPTOR:", word ) << endl;
			return false;
		}
		
		dom->setStateDescriptor( new StateDescriptor() );
		
		StateDescriptor *sd = dom->getStateDescriptor();
		
		if( ifs.eof() ){
			cerr << errorMessage( "DESCRIPTOR:", "END OF FILE" ) << endl;
			return false;
		}
		
		ifs >> word;
		
		while( !ifs.eof() && word != "ACTION:" ){
			var = word;
			ifs >> word;
			vtype = word;

			if( !sd->addStateVariable( var, vtype ) ){
				cerr << "Variable " + var + "(" + vtype + ") not added because its duplicated or its variable type is different from counter, const-pointer, pointer or register." ;
			}

			ifs >> word;
		} 
		
		// START WITH ACTIONS
		// change to while	
		while( !ifs.eof() && word == "ACTION:" ){
			ifs >> word; 
			
			if( word != "NAME:" ){
				cerr << errorMessage( "NAME:", word );	
				return false;
			}
			
			ifs >> name; // action name must be without spaces
			
			ifs >> word;
			if( word != "TYPE:" ){
				cerr << errorMessage( "TYPE:", word );	
				return false;
			}
			
			ifs >> atype; // action type must be either "math" or "memory"
			
			int max_val = MAX_VAL;
			if( atype == "math" )
				max_val = sd->getNumRegisters();
			
			dom->addAction( new Action( name, atype ) );
			Action *act = dom->getAction( name );

			ifs >> word;
			if( word != "PRECONDITIONS:" ){
				delete act;
				cerr << errorMessage( "PRECONDITIONS:", word );	
				return false;
			}

			// Parsing PRECONDITIONS expressions
			bool ok = true;
			ifs >> word;
			while( !ifs.eof() && ( word == "(" ) ){
				if( word != "(" ){
					ok = false;
					cerr << errorMessage( "(", word );	
					break;
				}

				string cond_op, var_num;
				ifs >> var >> cond_op >> var_num;
				Variable *lhs, *rhs; 
				
				if( (lhs = readVar( var, sd )) == NULL ){
					ok = false;
					cerr << "Failure parsing variable " << var << endl;
					break;
				}
				if( (rhs = readVar( var_num, sd )) == NULL ){
					ok = false;
					cerr << "Failure parsing variable " << var_num << endl;
					break;
				}

				Condition *cond;
				if( (cond = readCondition( cond_op, lhs, rhs, max_val ) ) == NULL ){
					cerr << "Failure parsing condition " << cond_op << endl;
				}
		
				act->addCondition( cond );
				
				ifs >> word;
				if( word != ")" ){
					ok = false;
					cerr << errorMessage( ")", word );	
					break;
				}
				
				ifs >> word;
			}
			if( !ok ){
				delete act;
				return false;
			}			
			
			// Parsing EFFECTS expressions
			if( word == "EFFECTS:" && !ifs.eof() )
				ifs >> word; 
			
			while( !ifs.eof() && word == "(" ){
				if( word != "(" ){
					ok = false;
					cerr << errorMessage( "(", word );	
					break;
				}

				string cond_op, var_num;
				ifs >> var >> cond_op >> var_num;

				Variable *lhs = NULL, *rhs = NULL;
				if( (lhs = readVar( var, sd )) == NULL ){
					ok = false;
					cerr << "Failure parsing variable " << var << endl;
					break;
				}
				if( (rhs = readVar( var_num, sd )) == NULL ){
					ok = false;
					cerr << "Failure parsing variable " << var_num << endl;
					break;
				}

				Operation *op;
				if( ( op = readOperation( cond_op, lhs, rhs ) ) == NULL ){
					ok = false;
					cerr << "Failure parsing condition " << cond_op << endl;
					break;
				}
		
				act->addOperation( op );
				
				ifs >> word;
				if( word != ")" ){
					ok = false;
					cerr << errorMessage( "(", word );	
					break;
				}
				
				ifs >> word;
			}
			
			if( !ok ){
				delete act;
				return false;
			}
			
		}
		
		ifs.close();
		
		return true;
	}
	
	bool parseInstance( Domain *dom, Instance *ins, const string &file_name = "" ){
		ifstream ifs( file_name.c_str() );
		
		// INSTANCE NAME
		string name;
		ifs >> name;
		
		ins->setName( name );

		string word;

		// STATE DESCRIPTOR
		ifs >> word;
		if( word != "INIT:" ){
			cerr << errorMessage( "INIT:", word ) << endl;
			return false;
		}
		
		ifs >> word;
		
		StateDescriptor *sd = dom->getStateDescriptor();
		
		ins->setInitialState( new State( sd->getNumCounters(), sd->getNumPointers(), sd->getNumConstPointers(), sd->getNumRegisters() ) );
		
		State *init = ins->getInitialState();
		
		while( !ifs.eof() && word == "(" ){
			string var;
			int value;
			ifs >> var >> word >> value >> word;
			int id = sd->getID( var );
			string vtype = sd->getVType( var );
			
			init->setVar( id, value, vtype );
			
			ifs >> word;
		}
		
		if( word != "GOAL:" ){
			cerr << errorMessage( "GOAL:", word ) << endl;
			return false;
		}
		
		ifs >> word;
		
		while( !ifs.eof() && word == "(" ){
			string var;
			string cond_op;
			string value;
			
			ifs >> var >> cond_op >> value >> word;
			
			Variable *lhs = readVar( var, sd );
			Variable *rhs = readVar( value, sd );
			Condition *cond = readCondition( cond_op, lhs, rhs );
			
			ins->addCondition( cond );
			
			ifs >> word;
		}

		ifs.close();
		
		return true;
	}
	
};

#endif
