[ ! -d "domain/" ] && mkdir -p "domain"
[ ! -d "domain/validation/" ] && mkdir -p "domain/validation"
[ ! -d "experiments/" ] && mkdir -p "experiments/"
[ ! -d "experiments/validation/" ] && mkdir -p "experiments/validation"

echo 'Generating Corridor validation instances...'
if [ -d "domain/validation/corridor/" ]; then
	rm domain/validation/corridor/*
else
	mkdir -p "domain/validation/corridor"
fi 
./domain/generators/corridor.py 12 1011 domain/validation/corridor/

echo 'Generating Fibonacci validation instances...'
if [ -d "domain/validation/fibo/" ]; then
	rm domain/validation/fibo/*
else
	mkdir -p "domain/validation/fibo"
fi 
./domain/generators/fibo.py 12 44 domain/validation/fibo/

echo 'Generating Find validation instances...'
if [ -d "domain/validation/find/" ]; then
	rm domain/validation/find/*
else
	mkdir -p "domain/validation/find"
fi 
./domain/generators/find.py 1000 1000 50000 domain/validation/find/

echo 'Generating Gripper validation instances...'
if [ -d "domain/validation/gripper/" ]; then
	rm domain/validation/gripper/*
else
	mkdir -p "domain/validation/gripper"
fi 
./domain/generators/gripper.py 12 1011 domain/validation/gripper/

echo 'Generating Reverse validation instances...'
if [ -d "domain/validation/reverse/" ]; then
	rm domain/validation/reverse/*
else
	mkdir -p "domain/validation/reverse"
fi 
#./domain/generators/reverse.py validation 100 10 1090 domain/validation/reverse/
./domain/generators/reverse.py validation 1000 1000 50000 domain/validation/reverse/

echo 'Generating Select validation instances...'
if [ -d "domain/validation/select/" ]; then
	rm domain/validation/select/*
else
	mkdir -p "domain/validation/select"
fi 
./domain/generators/select.py validation 1000 1000 50000 domain/validation/select/

echo 'Generating Sorting validation instances...'
if [ -d "domain/validation/sorting/" ]; then
	rm domain/validation/sorting/*
else
	mkdir -p "domain/validation/sorting"
fi 
#./domain/generators/sorting.py validation 12 1 111 domain/validation/sorting/
./domain/generators/sorting.py validation 12 1 31 domain/validation/sorting/

echo 'Generating Triangular Sum validation instances...'
if [ -d "domain/validation/triangular-sum/" ]; then
	rm -rf domain/validation/triangular-sum/
	mkdir -p "domain/validation/triangular-sum"
else
	mkdir -p "domain/validation/triangular-sum"
fi 
./domain/generators/triangular-sum.py 12 44720 domain/validation/triangular-sum/




