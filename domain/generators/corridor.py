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
	str_domain = "CORRIDOR\n\n"
	str_domain += "STATE DESCRIPTOR:\n"
	
	str_domain += "i pointer\n"
	str_domain += "gi const-pointer\n"
	str_domain += "v[0] register\n"
	str_domain += "v[1] register\n"
		
	str_domain += "\nACTION:\n"
	str_domain += "NAME: right(*i)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( i = 0 )\n"
	str_domain += "( *i + 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *i += 1 )\n"
	
	str_domain += "\nACTION:\n"
	str_domain += "NAME: left(*i)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( i = 0 )\n"
	str_domain += "( *i - 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *i -= 1 )\n"

	f_domain=open( out_folder + "domain.txt", "w" )
	f_domain.write( str_domain )
	f_domain.close
		

	random.seed(0)
	
	# INSTANCES
	for i in range(from_nth,to_nth+1):
		# Problem name
		str_problem = "CORRIDOR-" + str(i) + "\n"
		
		# Compute (pre)
		vi = random.randint(0,i)
		vgi = random.randint(0,i)
		while vgi == vi:
			vgi = random.randint(0, i)
		
		# Initial state
		str_problem += "\nINIT:\n"
		str_problem += "( i = 0 )\n"
		str_problem += "( gi = 1 )\n"
		str_problem += "( v[0] = " + str(vi) + " )\n"
		str_problem += "( v[1] = " + str(vgi) + " )\n"
		
		# Compute		
		
		# Goal condition
		str_problem += "\nGOAL:\n"
		str_problem += "( v[0] = " + str(vgi) + " )\n"
		str_problem += "( v[1] = " + str(vgi) + " )\n"
		
		#print( str_problem )
		f_problem=open( out_folder + str( i+1-from_nth ) + ".txt","w")
		f_problem.write( str_problem )
		f_problem.close()
	
	sys.exit( 0 )
	
if __name__ == "__main__":
	main()
