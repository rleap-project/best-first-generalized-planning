#!/usr/bin/env python
# coding: utf-8

import sys
import numpy as np
import random

def main():
	try:
		from_nth = int( sys.argv[1] )
		to_nth = int( sys.argv[2] )
		out_folder = sys.argv[3]
	except:
		print "Usage: "
		print sys.argv[ 0 ] + " <from_n> <to_n> <outfolder>"
		print sys.argv[ 0 ] + " 2 6 tmp/"
		sys.exit(-1)
		
	# DOMAIN
	str_domain = "GRIPPER\n\n"
	str_domain += "STATE DESCRIPTOR:\n"
	
	str_domain += "left-arm pointer\n"
	str_domain += "right-arm pointer\n"
	str_domain += "robot-at pointer\n"
	str_domain += "last-object const-pointer\n"
	for i in range(0,to_nth+1):
		str_domain += "v["+str(i)+"] register\n"
	
	str_domain += "\nACTION:\n"
	str_domain += "NAME: pick(*left-arm)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( left-arm < last-object )\n"
	str_domain += "( left-arm > 0 )\n"
	str_domain += "( robot-at = 0 )\n"	
	str_domain += "( *left-arm = *robot-at )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *left-arm = 2 )\n"
	
	str_domain += "\nACTION:\n"
	str_domain += "NAME: pick(*right-arm)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( right-arm < last-object )\n"
	str_domain += "( right-arm > 0 )\n"
	str_domain += "( robot-at = 0 )\n"	
	str_domain += "( *right-arm = *robot-at )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *right-arm = 3 )\n"
	
	str_domain += "\nACTION:\n"
	str_domain += "NAME: drop(*left-arm)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( left-arm < last-object )\n"
	str_domain += "( left-arm > 0 )\n"
	str_domain += "( robot-at = 0 )\n"	
	str_domain += "( *left-arm = 2 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *left-arm = *robot-at )\n"
	
	str_domain += "\nACTION:\n"
	str_domain += "NAME: drop(*right-arm)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( right-arm < last-object )\n"
	str_domain += "( right-arm > 0 )\n"
	str_domain += "( robot-at = 0 )\n"	
	str_domain += "( *right-arm = 3 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *right-arm = *robot-at )\n"
	
	str_domain += "\nACTION:\n"
	str_domain += "NAME: moveAB()\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( robot-at = 0 )\n"	
	str_domain += "( *robot-at = 0 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *robot-at = 1 )\n"
	
	str_domain += "\nACTION:\n"
	str_domain += "NAME: moveBA()\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( robot-at = 0 )\n"	
	str_domain += "( *robot-at = 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *robot-at = 0 )\n"
	
	f_domain=open( out_folder + "domain.txt", "w" )
	f_domain.write( str_domain )
	f_domain.close
			
	# INSTANCES
	for i in range(from_nth,to_nth+1):
		# Problem name
		str_problem = "GRIPPER-" + str(i) + "\n"
				
		# Initial state
		str_problem += "\nINIT:\n"
		str_problem += "( left-arm = 1 )\n"
		str_problem += "( right-arm = 1 )\n"
		str_problem += "( robot-at = 0 )\n"
		str_problem += "( last-object = " + str(i) + " )\n"
		
		for j in range(0,i+1):
			str_problem += "( v[" + str(j) + "] = 0 )\n"
		
		# Compute		
		
		# Goal condition
		str_problem += "\nGOAL:\n"
		for j in range(1,i):
			str_problem += "( v[" + str(j) + "] = 1 )\n"
		
		#print( str_problem )
		f_problem=open( out_folder + str( i+1-from_nth ) + ".txt","w")
		f_problem.write( str_problem )
		f_problem.close()
	
	sys.exit( 0 )
	
if __name__ == "__main__":
	main()
