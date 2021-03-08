g++ -Wall -O3 -std=c++11 -ggdb3 src/main.cpp -o main.bin
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./main.bin 5 domain/triangular-sum/
