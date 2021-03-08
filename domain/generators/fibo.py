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
	str_domain = "FIBONACCI-2\n\n"
	str_domain += "STATE DESCRIPTOR:\n"
	str_domain += "n const-pointer\n"
	str_domain += "b pointer\n"
	str_domain += "c pointer\n"
	for i in range(to_nth+1):
		str_domain += "v["+str(i)+"] register\n"
		
	str_domain += "\nACTION:\n"
	str_domain += "NAME: add(*b,*c)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *b + *c )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *b += *c )\n"

	str_domain += "\nACTION:\n"
	str_domain += "NAME: add(*c,*b)\n"
	str_domain += "TYPE: memory\n"
	str_domain += "PRECONDITIONS:\n"
	str_domain += "( *c + *b )\n"
	str_domain += "EFFECTS:\n"
	str_domain += "( *c += *b )\n\n"

	f_domain=open( out_folder + "domain.txt", "w" )
	f_domain.write( str_domain )
	f_domain.close
		
	# INSTANCES
	for i in range(from_nth,to_nth+1):
		# Problem name
		str_problem = "FIBO-" + str(i) + "\n"
		
		# Initial state
		str_problem += "\nINIT:\n"
		str_problem += "( n = " + str(i) + " )\n"
		str_problem += "( b = 0 )\n"
		str_problem += "( c = 0 )\n"
		str_problem += "( v[0] = 0 )\n"
		str_problem += "( v[1] = 1 )\n"
		for j in range( 2, i+1 ):
			str_problem += "( v[" + str(j) + "] = 0 )\n"
		
		# Goal condition
		str_problem += "\nGOAL:\n"
		str_problem += "( v[0] = 0 )\n"
		str_problem += "( v[1] = 1 )\n"
		fn = 1
		fn1 = 0
		fn2 = 0
		for j in range( 2, i+1 ):
			fn2 = fn1
			fn1 = fn
			fn += fn2
			str_problem += "( v[" + str(j) + "] = " + str(fn) + " )\n"
		
		#print( str_problem )
		f_problem=open( out_folder + str( i+1-from_nth ) + ".txt","w")
		f_problem.write( str_problem )
		f_problem.close()
	
	sys.exit( 0 )
	
if __name__ == "__main__":
	main()
