echo 'Generating folders...'
[ ! -d "domain/" ] && mkdir -p "domain"
[ ! -d "domain/heuristics/" ] && mkdir -p "domain/heuristics"
[ ! -d "domain/heuristics/corridor/" ] && mkdir -p "domain/heuristics/corridor"
[ ! -d "domain/heuristics/fibo/" ] && mkdir -p "domain/heuristics/fibo"
[ ! -d "domain/heuristics/find/" ] && mkdir -p "domain/heuristics/find"
[ ! -d "domain/heuristics/gripper/" ] && mkdir -p "domain/heuristics/gripper"
[ ! -d "domain/heuristics/reverse/" ] && mkdir -p "domain/heuristics/reverse"
[ ! -d "domain/heuristics/select/" ] && mkdir -p "domain/heuristics/select"
[ ! -d "domain/heuristics/sorting/" ] && mkdir -p "domain/heuristics/sorting"
[ ! -d "domain/heuristics/triangular-sum/" ] && mkdir -p "domain/heuristics/triangular-sum"
[ ! -d "domain/validation/" ] && mkdir -p "domain/validation"
[ ! -d "domain/validation/corridor/" ] && mkdir -p "domain/validation/corridor"
[ ! -d "domain/validation/fibo/" ] && mkdir -p "domain/validation/fibo"
[ ! -d "domain/validation/find/" ] && mkdir -p "domain/validation/find"
[ ! -d "domain/validation/gripper/" ] && mkdir -p "domain/validation/gripper"
[ ! -d "domain/validation/reverse/" ] && mkdir -p "domain/validation/reverse"
[ ! -d "domain/validation/select/" ] && mkdir -p "domain/validation/select"
[ ! -d "domain/validation/sorting/" ] && mkdir -p "domain/validation/sorting"
[ ! -d "domain/validation/triangular-sum/" ] && mkdir -p "domain/validation/triangular-sum"

[ ! -d "experiments/" ] && mkdir -p "experiments"
[ ! -d "experiments/heuristics/" ] && mkdir -p "experiments/heuristics"
[ ! -d "experiments/validation/" ] && mkdir -p "experiments/validation"

echo 'Compiling the main project...'
./scripts/compile_main.sh
echo 'Compiling the validator...'
./scripts/compile_validator.sh
