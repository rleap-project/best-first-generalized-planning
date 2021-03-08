#!/usr/bin/env python
# coding: utf-8

import sys
import numpy as np
import random

def main():
	try:
		from_size = int( sys.argv[1] )
		step = int( sys.argv[2] )
		to_size = int( sys.argv[3] )
		out_folder = sys.argv[4]
	except:
		print "Usage (from_size > 19 and step > 0): "
		print sys.argv[ 0 ] + " <from_size> <step> <to_size> <out_folder>"
		print sys.argv[ 0 ] + " 10 5 100 tmp/"
		sys.exit(-1)
	
	if step < 1 or to_size < from_size :
		sys.exit( -2 )
	
	# DOMAIN
	str_domain = "FIND\n\n"
	str_domain += "STATE DESCRIPTOR:\n"
	str_domain += "tail const-pointer\n"
	str_domain += "accumulator counter\n"
	str_domain += "a pointer\n"
	str_domain += "target pointer\n"
	for i in range(to_size+1):
		str_domain += "v["+str(i)+"] register\n"

	f_domain=open( out_folder + "domain.txt", "w" )
	f_domain.write( str_domain )
	f_domain.close
			
	# INSTANCES
	np.random.seed(1007)
	i = from_size
	while i <= to_size :
		# Computing
		v = np.random.randint(i//2, size=i)	
		tg = np.random.randint(0,i-1)
		acc = sum( v == v[tg] )
		
		# Problem name
		str_problem = "FIND-" + str(i) + "\n"
		
		# Initial state
		str_problem += "\nINIT:\n"
		str_problem += "( tail = " + str(i) + " )\n"
		str_problem += "( a = 0 )\n"
		str_problem += "( accumulator = 0 )\n"
		str_problem += "( target = " + str(tg) + " )\n"
		for j in range(i):
			str_problem += "( v["+str(j)+"] = " + str(v[j]) + " )\n"
		
		# Goal condition
		str_problem += "\nGOAL:\n"
		str_problem += "( a = " + str(i) + " )\n"
		str_problem += "( accumulator = " + str(acc) + " )\n"
		str_problem += "( target = " + str(tg) + " )\n"
		
		#print( str_problem )
		f_problem=open( out_folder + str( (i - from_size + step)//step ) + ".txt","w")
		f_problem.write( str_problem )
		f_problem.close()
		
		i += step
		
	sys.exit( 0 )
	
if __name__ == "__main__":
	main()
