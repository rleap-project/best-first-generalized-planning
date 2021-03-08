domains=("triangular-sum" "reverse" "select" "find" "corridor" "gripper" "fibo" "sorting")
bounds=(5 7 7 7 7 8 8 9)
heuristics=("h1" "h2" "h3" "h4" "h5" "h6")

total=${#domains[@]}
#total=1
for (( i=0; i<$(($total)); i++ ))
do
	for h in "${heuristics[@]}"
	do
		./scripts/time_memory.sh ./main.bin ${bounds[i]} "domain/heuristics/${domains[i]}/" $h
	done
    experiments_file=$experiments_file"_${argv[j]}"
done


