#!/usr/bin/env python
# coding: utf-8

import sys

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
	str_domain = "TRIANGULAR-SUM\n\n"
	str_domain += "STATE DESCRIPTOR:\n"
	str_domain += "a pointer\n"
	str_domain += "b pointer\n"
	for i in range(2):
		str_domain += "v["+str(i)+"] register\n"
		
	str_domain += "\nACTION:\n"
	str_domain += "NAME: add(*a,*b)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *a + *b )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *a += *b )\n"

	str_domain += "\nACTION:\n"
	str_domain += "NAME: add(*b,*a)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *b + *a )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *b += *a )\n\n"

	str_domain += "\nACTION:\n"
	str_domain += "NAME: inc(*a)\n"
	str_domain += "TYPE: math\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *a + 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *a += 1 )\n\n"

	str_domain += "\nACTION:\n"
	str_domain += "NAME: inc(*b)\n"
	str_domain += "TYPE: math\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *b + 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *b += 1 )\n\n"

	str_domain += "\nACTION:\n"
	str_domain += "NAME: dec(*a)\n"
	str_domain += "TYPE: math\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *a - 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *a -= 1 )\n\n"

	str_domain += "\nACTION:\n"
	str_domain += "NAME: dec(*b)\n"
	str_domain += "TYPE: math\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *b - 1 )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *b -= 1 )\n\n"

	f_domain=open( out_folder + "domain.txt", "w" )
	f_domain.write( str_domain )
	f_domain.close
		
	# INSTANCES
	for i in range(from_nth,to_nth+1):
		# Problem name
		str_problem = "TRIANGULAR-SUM-" + str(i) + "\n"
		
		# Initial state
		str_problem += "\nINIT:\n"
		str_problem += "( a = 0 )\n"
		str_problem += "( b = 0 )\n"
		str_problem += "( v[0] = 0 )\n"
		str_problem += "( v[1] = " + str(i) + " )\n"
		
		# Compute
		sol = i*(i+1)/2
		
		# Goal condition
		str_problem += "\nGOAL:\n"
		str_problem += "( v[0] = " + str(sol) + " )\n"
		str_problem += "( v[1] = 0 )\n"
		
		#print( str_problem )
		f_problem=open( out_folder + str( i+1-from_nth ) + ".txt","w")
		f_problem.write( str_problem )
		f_problem.close()
	
	sys.exit( 0 )
	
if __name__ == "__main__":
	main()
