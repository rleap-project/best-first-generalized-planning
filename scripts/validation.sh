echo 'Validation with INFINITE detection...'
#echo 'Random FIBO...'
#./scripts/time_memory.sh ./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ INF

echo 'Random CORRIDOR...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/corridor_h5_h1.prog domain/validation/corridor/ INF

echo 'Random FIBO...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ INF

echo 'Random FIND...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/find_h5_h1.prog domain/validation/find/ INF

echo 'Random GRIPPER...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/gripper_h5_h1.prog domain/validation/gripper/ INF

echo 'Random REVERSE...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/reverse_h5_h1.prog domain/validation/reverse/ INF

echo 'Random SELECT...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/select_h5_h1.prog domain/validation/select/ INF

echo 'Random SORTING...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/sorting_h5_h1.prog domain/validation/sorting/ INF

echo 'Random TRIANGULAR-SUM...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/triangular-sum_h5_h1.prog domain/validation/triangular-sum/ INF



echo 'Validation without INFINITE detection...'
#echo 'Random FIBO...'
#./scripts/time_memory.sh ./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ NOINF

echo 'Random CORRIDOR...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/corridor_h5_h1.prog domain/validation/corridor/ NOINF

echo 'Random FIBO...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ NOINF

echo 'Random FIND...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/find_h5_h1.prog domain/validation/find/ NOINF

echo 'Random GRIPPER...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/gripper_h5_h1.prog domain/validation/gripper/ NOINF

echo 'Random REVERSE...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/reverse_h5_h1.prog domain/validation/reverse/ NOINF

echo 'Random SELECT...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/select_h5_h1.prog domain/validation/select/ NOINF

echo 'Random SORTING...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/sorting_h5_h1.prog domain/validation/sorting/ NOINF

echo 'Random TRIANGULAR-SUM...'
./scripts/time_memory.sh ./validator.bin experiments/heuristics/triangular-sum_h5_h1.prog domain/validation/triangular-sum/ NOINF
