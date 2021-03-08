domains=("triangular-sum" "reverse" "select" "find" "corridor" "gripper" "fibo" "sorting")
bounds=(5 7 7 7 7 8 8 9)
heuristics=("h1" "h2" "h3")

total=${#domains[@]}
#total=1
for (( i=0; i<$(($total)); i++ ))
do
	for h in "${heuristics[@]}"
	do
		echo "Running ${domains[i]} h5 $h..."
		./scripts/time_memory.sh ./main.bin ${bounds[i]} "domain/heuristics/${domains[i]}/" h5 $h
		echo "Running ${domains[i]} $h h5..."
		./scripts/time_memory.sh ./main.bin ${bounds[i]} "domain/heuristics/${domains[i]}/" $h h5
	done
    experiments_file=$experiments_file"_${argv[j]}"
done



