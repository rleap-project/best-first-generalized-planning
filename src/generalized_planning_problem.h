#ifndef __GENERALIZED_PLANNING_PROBLEM_H__
#define __GENERALIZED_PLANNING_PROBLEM_H__

#include "common.h"
#include "domain.h"
#include "instance.h"

class GeneralizedPlanningProblem{
public:
	GeneralizedPlanningProblem(){
	}
	
	~GeneralizedPlanningProblem(){
		if( _domain ) delete _domain;
		for( int i = 0; i < int( _instances.size() ); i++ )
			if( _instances[ i ] )
				delete _instances[ i ];
	}
	
	void addDomain( Domain *dom ){
		_domain = dom;
	}
	
	Domain* getDomain(){
		return _domain;
	}
	
	void addInstance( Instance *ins ){
		_instances.push_back( ins );
	}	
	
	Instance* getInstance( int id ){
		return _instances[ id ];
	}
	
	vector< Instance* > getInstances(){
		return _instances;
	}
	
	int getNumInstances(){
		return int( _instances.size() );
	}
	
	string toString(){
		string ret = "[GENERALIZED PLANNING PROBLEM]:\n";
		
		if( _domain ) 
			ret += _domain->toString();
		
		for( int i = 0; i < int( _instances.size() ); i++ )
			ret += "\n\n" + _instances[ i ]->toString();
		
		return ret;
	}
	
private:
	Domain* _domain;
	vector< Instance* > _instances;
};

#endif

