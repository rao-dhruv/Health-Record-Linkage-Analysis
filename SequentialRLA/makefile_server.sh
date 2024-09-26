g++ -std=c++17 -O3 -I /usr/local/boost_1_80_0/ rla_s.cpp -o rlapp
./rlapp amazon_standardized.csv
# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt  ./rlapp amazon_standardized.csv