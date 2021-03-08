[ ! -d "domain/" ] && mkdir -p "domain"
[ ! -d "domain/heuristics/" ] && mkdir -p "domain/heuristics"
[ ! -d "experiments/" ] && mkdir -p "experiments"
[ ! -d "experiments/heuristics/" ] && mkdir -p "experiments/heuristics"


echo 'Generating Corridor synthesis instances...'
if [ -d "domain/heuristics/corridor/" ]; then
	rm domain/heuristics/corridor/*
else
	mkdir -p "domain/heuristics/corridor"
fi 
./domain/generators/corridor.py 2 21 domain/heuristics/corridor/

echo 'Generating Fibonacci synthesis instances...'
if [ -d "domain/heuristics/fibo/" ]; then
	rm domain/heuristics/fibo/*
else
	mkdir -p "domain/heuristics/fibo"
fi 
./domain/generators/fibo.py 2 11 domain/heuristics/fibo/

echo 'Generating Find synthesis instances...'
if [ -d "domain/heuristics/find/" ]; then
	rm domain/heuristics/find/*
else
	mkdir -p "domain/heuristics/find"
fi 
./domain/generators/find.py 2 1 21 domain/heuristics/find/

echo 'Generating Gripper synthesis instances...'
if [ -d "domain/heuristics/gripper/" ]; then
	rm domain/heuristics/gripper/*
else
	mkdir -p "domain/heuristics/gripper"
fi 
./domain/generators/gripper.py 2 21 domain/heuristics/gripper/

echo 'Generating Reverse synthesis instances...'
if [ -d "domain/heuristics/reverse/" ]; then
	rm domain/heuristics/reverse/*
else
	mkdir -p "domain/heuristics/reverse"
fi 
./domain/generators/reverse.py synthesis 2 1 21 domain/heuristics/reverse/

echo 'Generating Select synthesis instances...'
if [ -d "domain/heuristics/select/" ]; then
	rm domain/heuristics/select/*
else
	mkdir -p "domain/heuristics/select"
fi 
./domain/generators/select.py synthesis 2 1 21 domain/heuristics/select/

echo 'Generating Sorting synthesis instances...'
if [ -d "domain/heuristics/sorting/" ]; then
	rm domain/heuristics/sorting/*
else
	mkdir -p "domain/heuristics/sorting"
fi 
./domain/generators/sorting.py synthesis 2 1 11 domain/heuristics/sorting/

echo 'Generating Triangular Sum synthesis instances...'
if [ -d "domain/heuristics/triangular-sum/" ]; then
	rm domain/heuristics/triangular-sum/*
else
	mkdir -p "domain/heuristics/triangular-sum"
fi 
./domain/generators/triangular-sum.py 2 11 domain/heuristics/triangular-sum/

