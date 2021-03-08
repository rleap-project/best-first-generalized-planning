# Best First Generalized Planning
Heuristic and evaluation techniques for searching programs that generally solve planning problems.
This framework uses a domain specific language for planning with counters, constant pointers, pointers and memory registers.
It starts with a short tutorial, and later explains every step in more detail.

## Short Tutorial

1. Generate folders and compile the project.
```shell
./scripts/compile_all.sh
```
2. Generate synthesis and validation instances of Fibonacci.
```shell
./domain/generators/fibo.py 2 11 domain/heuristics/fibo/
./domain/generators/fibo.py 12 44 domain/validation/fibo/
```
3. Search a program that solves Fibonacci
```shell
./main.bin 8 domain/heuristics/fibo/
```
The resulting program should looks like:
```shell
0. inc(c)
1. inc(c)
2. add(*c,*b)
3. inc(b)
4. add(*c,*b)
5. cmp(n,c)
6. goto(1,!(( zf = 1 )( cf = 0 )))
7. end
```
4. Validate the output program, with and without infinite detection.
```shell
./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ INF
./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ NOINF
```
Both validations should finish with an `[INFO] GOAL ACHIEVED!`

## Required folders and project compilation
The next command is used to generate required folders and compile the project:

```shell
./scripts/compile_all.sh
```
The compilation will generate a ```main.bin``` and ```validator.bin``` executables file in the root folder.

## Generating random inputs

The inputs are randomly generated from a high-level specification.
The next are the examples used in the project to generate the original domains and instances:

```shell
./domain/generators/corridor.py 2 21 domain/heuristics/corridor/
./domain/generators/fibo.py 2 11 domain/heuristics/fibo/
./domain/generators/find.py 2 1 21 domain/heuristics/find/
./domain/generators/gripper.py 2 21 domain/heuristics/gripper/
./domain/generators/reverse.py synthesis 2 1 21 domain/heuristics/reverse/
./domain/generators/select.py synthesis 2 1 21 domain/heuristics/select/
./domain/generators/sorting.py synthesis 2 1 11 domain/heuristics/sorting/
./domain/generators/triangular-sum.py 2 11 domain/heuristics/triangular-sum/
```

## Searching a program
Here we show few examples which loads (h5,f1) heuristic and evaluation functions by default:

```shell
./main.bin 5 domain/heuristics/triangular-sum/
./main.bin 7 domain/heuristics/corridor/
./main.bin 7 domain/heuristics/select/
./main.bin 7 domain/heuristics/find/
./main.bin 7 domain/heuristics/reverse/
./main.bin 8 domain/heuristics/fibo/
./main.bin 8 domain/heuristics/gripper/
./main.bin 9 domain/heuristics/sorting/
```

Explicit load of another function should be appended to the arguments as follows:
```shell
./main.bin 7 domain/heuristics/select/ h2
```

Or a list of functions:
```shell
./main.bin 9 domain/heuristics/sorting/ h3 h5 h1
```

## Validating a program

Generating validation instances:
```shell
./domain/generators/corridor.py 12 1011 [folder]
```

Other instance generators:
```shell
./domain/generators/fibo.py 12 44 [folder]
./domain/generators/find.py 1000 1000 50000  [folder]
./domain/generators/gripper.py 12 1011 domain/validation/gripper/
./domain/generators/reverse.py validation 1000 1000 50000 [folder]
./domain/generators/select.py validation 1000 1000 50000 [folder]
./domain/generators/sorting.py validation 12 1 31 [folder]
./domain/generators/triangular-sum.py 12 44720 [folder]
```


Validating a specific program with infinite program detection by default:
```shell
./validator.bin programs/fibo.prog domain/validation/fibo/
```

Explicit validation with/out infinite program detection (after computing Fibonacci with h5 h1 functions):
```shell
./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ INF
./validator.bin experiments/heuristics/fibo_h5_h1.prog domain/validation/fibo/ NOINF
```

## Reproducing ICAPS-21 Experiments

> This is a remainder that some experiments, like validation with infinite detection, may exhaust your computer resources.

In order to reproduce ICAPS21 experiments simply run:
```shell
./scripts/icaps21_experiments.sh
```

This script consist of the following commands which should succeed (given the paper settings):
```shell
./scripts/compile_all.sh
./scripts/synthesis_generators.sh
./scripts/heuristics_study.sh
./scripts/synthesis_supplementary.sh
./scripts/validation_generators.sh
./scripts/validation.sh
```
